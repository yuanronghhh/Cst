#include <Framework/Media/FrMediaDecoder.h>
#include <Framework/Media/FrMediaFrame.h>
#include <Framework/Media/FrMediaStream.h>
#include <Framework/Media/FrMediaFile.h>
#include <Framework/Media/FrAudioDecoder.h>
#include <Framework/Media/FrVideoDecoder.h>
#include <Framework/Media/FrMediaPacket.h>
#include <Framework/Media/FrMediaPipeline.h>

static const SysChar* DECODER_NAMES[] = {
  "video_decoder",
  "audio_decoder",
  "media_data",
  "media_subtitle",
  "media_attachment",
  "media_nb",
  "media_packet"
};

SYS_DEFINE_TYPE(FrMediaDecoder, fr_media_decoder, FR_TYPE_DECODER);

SysType fr_media_decoder_enum_to_type(FR_MEDIA_ENUM mediaType) {
  switch (mediaType) {
  case FR_MEDIA_VIDEO:
    return FR_TYPE_VIDEO_DECODER;
  case FR_MEDIA_AUDIO:
    return FR_TYPE_AUDIO_DECODER;
  default:
    return 0;
  }
}

SysBool fr_media_decoder_get_hw_info(
    FrMediaDecoder *self,
    SysInt hw_dtype, 
    SysInt *hw_pix_format) {

  SysInt i;
  const AVCodecHWConfig *config;

  for (i = 0; ;i++) {
    config = avcodec_get_hw_config(self->codec, i);
    if(config == NULL) { return false; }

    if ((config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX)
        && config->device_type == hw_dtype) {
      *hw_pix_format = config->pix_fmt;
      break;
    }
  }

  return true;
}

static const SysChar* fr_media_decoder_type_to_name(SysType tp) {
  if(tp == FR_TYPE_VIDEO_DECODER) {
    return DECODER_NAMES[FR_MEDIA_VIDEO];
  }

  if(tp == FR_TYPE_AUDIO_DECODER) {
    return DECODER_NAMES[FR_MEDIA_AUDIO];
  }

  return NULL;
}

void fr_media_decoder_flush(FrMediaDecoder* self) {
  sys_return_if_fail(self != NULL);

  avcodec_flush_buffers(self->ctx);
}

SysInt fr_media_decoder_close_i(FrDecoder* o) {
  FrMediaDecoder *self = FR_MEDIA_DECODER(o);

  return avcodec_close(self->ctx);
}

SysInt fr_media_decoder_open_i(FrDecoder* o) {
  FrMediaDecoder *self = FR_MEDIA_DECODER(o);
  sys_return_val_if_fail(self != NULL, -1);

  return avcodec_open2(self->ctx, self->codec, NULL);
}

SysInt fr_media_decoder_send_packet(FrMediaDecoder* self,
    FrMediaPacket *pkt) {
  sys_return_val_if_fail(self != NULL, -1);

  return fr_media_avcodec_try_send_packet(self->ctx, pkt->ctx);
}

SysInt fr_media_decoder_receive_frame(
    FrMediaDecoder* self,
    FrMediaFrame **nframe) {

  sys_return_val_if_fail(self != NULL, -1);
  sys_return_val_if_fail(*nframe == NULL, -1);

  SysInt err;
  FrMediaFrame* frame = self->frame;

  err = fr_media_avcodec_try_receive_frame(self->ctx,
      frame->ctx);

  if (err < 0) {
    if(err == FR_MEDIA_ERROR_EOF) {
      fr_decoder_set_eof(FR_DECODER(self), true);
      avcodec_flush_buffers(self->ctx);
    }

    return err;
  }
  fr_media_frame_init_frame(frame);
  fr_media_stream_init_frame(self->stream, frame);
  *nframe = frame;

  return err;
}

SysInt fr_media_decoder_decode_frame_i(
    FrMediaDecoder* self,
    FrMediaFrame **nframe) {

  return fr_media_decoder_receive_frame(self, nframe);
}

static void media_decoder_create_context(FrMediaDecoder *self,
    FrMediaStream *stream) {
  AVStream *as = fr_media_stream_get_ctx(stream);

  self->stream = sys_object_ref(stream);
  self->codec = fr_media_find_decoder(as);
  self->ctx = fr_media_create_avcodec_context(self->codec, as);
}

void fr_media_decoder_construct(FrMediaDecoder *self,
    FrDecoderContext *info, 
    FrMediaStream *ms) {
  sys_return_if_fail(self != NULL);

  FrMediaDecoderClass* cls = FR_MEDIA_DECODER_GET_CLASS(self);
  sys_return_if_fail(cls->construct);

  cls->construct(self, info, ms);
}

FrDecoder* fr_media_decoder_new_by_type(
    SysType tp,
    const SysChar *name,
    FrMediaStream *ms) {
  FrMediaDecoder *o;

  o = sys_object_new(tp, NULL);
  FrDecoderContext info = { .name = (SysChar *)name };
  fr_media_decoder_construct(o, &info, ms);

  return FR_DECODER(o);
}

FrDecoder* fr_media_decoder_create_by_media_type(FrMediaFile* file,
    FR_MEDIA_ENUM mediaType) {

  FrMediaStream* mst;
  FrDecoder* o;
  SysType tp;
  const SysChar *name;

  mst = fr_media_file_stream_by_type(file, mediaType);
  tp = fr_media_decoder_enum_to_type(mediaType);
  if (tp == 0) { return NULL; }

  name = fr_media_decoder_type_to_name(tp);
  o = fr_media_decoder_new_by_type(tp, name, mst);

  return o;
}

SysInt fr_media_decoder_decode_frame(
    FrMediaDecoder *self,
    FrMediaFrame **frame) {
  sys_return_val_if_fail(self != NULL, -1);

  FrMediaDecoderClass* cls = FR_MEDIA_DECODER_GET_CLASS(self);
  sys_return_val_if_fail(cls->construct, -1);

  return cls->decode_frame(self, frame);
}

void fr_media_decoder_set_frame(FrMediaDecoder *self, FrMediaFrame * frame) {
  sys_return_if_fail(self != NULL);

  self->frame = frame;
}

FrMediaFrame * fr_media_decoder_get_frame(FrMediaDecoder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->frame;
}

/**
 * fr_media_decoder_try_decode_frame:
 *   decode and skip empty frame
 * @self:
 * @frame:
 *
 * Returns: SysInt
 */
SysInt fr_media_decoder_try_decode_frame(
    FrMediaDecoder *self,
    FrMediaFrame **frame) {
  SysInt err;

  FrMediaDecoderClass* cls = FR_MEDIA_DECODER_GET_CLASS(self);

  err = cls->decode_frame(self, frame);
  if (err >= 0) {

    return err;
  }

  if (err == FR_MEDIA_ERROR_AGAIN 
      || err == FR_MEDIA_ERROR_EOF) {

  } else {

    sys_warning_N("%d,%s", err, av_err2str(err));
  }

  return err;
}

void fr_media_decoder_set_frame_type(FrMediaDecoder* self, SysType tp) {
  sys_return_if_fail(tp > 0);
  sys_return_if_fail(self != NULL);

  self->frame_type = tp;
}

/* object api */
static void media_decoder_construct(FrMediaDecoder* self,
  FrDecoderContext *info,
  FrMediaStream* ms) {
  FrDecoder* o = FR_DECODER(self);

  FR_DECODER_CLASS(fr_media_decoder_parent_class)->construct(o, info);

  media_decoder_create_context(self, ms);
  self->frame = sys_object_new(self->frame_type, NULL);
}

FrDecoder* fr_media_decoder_new(void) {
  return sys_object_new(FR_TYPE_MEDIA_DECODER, NULL);
}

static void fr_media_decoder_dispose(SysObject* o) {
  FrMediaDecoder *self = FR_MEDIA_DECODER(o);
  FrDecoder *decoder = FR_DECODER(o);

  if (avcodec_is_open(self->ctx)) {

    fr_media_decoder_close_i(decoder);
  }
  avcodec_free_context(&self->ctx);
  sys_object_unref(self->stream);

  SYS_OBJECT_CLASS(fr_media_decoder_parent_class)->dispose(o);
}

static void fr_media_decoder_class_init(FrMediaDecoderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrDecoderClass *dcls = FR_DECODER_CLASS(cls);

  cls->construct = media_decoder_construct;
  cls->decode_frame = fr_media_decoder_decode_frame_i;

  dcls->open = fr_media_decoder_open_i;
  dcls->close = fr_media_decoder_close_i;

  ocls->dispose = fr_media_decoder_dispose;
}

void fr_media_decoder_init(FrMediaDecoder* self) {
}
