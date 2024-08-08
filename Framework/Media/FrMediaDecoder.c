#include <Framework/Media/FrMediaDecoder.h>
#include <Framework/Media/FrMediaFrame.h>
#include <Framework/Media/FrMediaStream.h>
#include <Framework/Media/FrMediaFile.h>
#include <Framework/Media/FrAudioDecoder.h>
#include <Framework/Media/FrVideoDecoder.h>
#include <Framework/Media/FrMediaPacket.h>

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

FrMediaStream *fr_media_decoder_get_stream(FrMediaDecoder *self) {

  return self->stream;
}

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

  return 0;
}

SysInt fr_media_decoder_open_i(FrDecoder* o) {
  FrMediaDecoder *self = FR_MEDIA_DECODER(o);
  sys_return_val_if_fail(self != NULL, -1);

  return avcodec_open2(self->ctx, self->codec, NULL);
}

void fr_media_decoder_write(FrMediaDecoder *self, FrMediaPacket *pkt) {
  sys_return_if_fail(self != NULL);

  FrMediaDecoderClass* cls = FR_MEDIA_DECODER_GET_CLASS(self);
  sys_return_if_fail(cls->write);

  cls->write(self, pkt);
}

SysInt fr_media_decoder_read(FrMediaDecoder *self, FrMediaPacket **pkt) {
  sys_return_val_if_fail(self != NULL, -1);

  FrMediaDecoderClass* cls = FR_MEDIA_DECODER_GET_CLASS(self);
  sys_return_val_if_fail(cls->read, -1);

  return cls->read(self, pkt);
}

SysInt fr_media_decoder_read_i(FrMediaDecoder *self, FrMediaPacket **pkt) {
  FrMediaPacket *npkt;

  npkt = sys_async_queue_try_pop(&self->queue);
  *pkt = npkt;

  return npkt != NULL;
}

SysInt fr_media_decoder_write_i(FrMediaDecoder *self, FrMediaPacket *pkt) {

  sys_async_queue_push(&self->queue, pkt);

  return 0;
}

SysInt fr_media_decoder_decode_frame_i(
    FrMediaDecoder* self,
    FrMediaFrame **nframe) {

  return fr_media_decoder_receive_frame(self, nframe);
}

SysInt fr_media_decoder_decode_frame(
    FrMediaDecoder *self,
    FrMediaFrame **frame) {
  sys_return_val_if_fail(self != NULL, -1);

  FrMediaDecoderClass* cls = FR_MEDIA_DECODER_GET_CLASS(self);
  sys_return_val_if_fail(cls->decode_frame, -1);

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
static void fr_media_decoder_construct_i(FrMediaDecoder* self,
  FrMediaDecoderContext *minfo) {
  FrDecoder* o = FR_DECODER(self);

  FrDecoderContext info = { .name = minfo->name };
  FR_DECODER_CLASS(fr_media_decoder_parent_class)->construct(o, &info);

  self->stream = sys_object_ref(minfo->media_stream);
  self->frame = sys_object_new(self->frame_type, NULL);

  fr_media_decoder_create(self, minfo);
}

FrDecoder* fr_media_decoder_new(void) {
  return sys_object_new(private_g_type, NULL);
}

FrDecoder *fr_media_decoder_new_I(FrMediaDecoderContext *info) {
  FrDecoder *o;

  sys_return_val_if_fail(info != 0, NULL);
  sys_return_val_if_fail(info->type != 0, NULL);
  sys_return_val_if_fail(info->name != NULL, NULL);
  sys_return_val_if_fail(info->media_stream != NULL, NULL);

  o = sys_object_new(info->type, NULL);
  fr_media_decoder_construct_i((FrMediaDecoder *)o, info);

  return o;
}

FrDecoder* fr_media_decoder_create_by_media_type(
    FrMediaFile* file,
    FR_MEDIA_ENUM mediaType) {

  FrMediaStream* mst;
  FrDecoder* o;
  SysType tp;
  const SysChar *name;

  mst = fr_media_file_stream_by_type(file, mediaType);
  tp = fr_media_decoder_enum_to_type(mediaType);
  if (tp == 0) { return NULL; }

  name = fr_media_decoder_type_to_name(tp);

  FrMediaDecoderContext info = { .name = name, .type = tp, .media_stream = mst };
  o = fr_media_decoder_new_I(&info);

  return o;
}


static void fr_media_decoder_dispose(SysObject* o) {
  FrMediaDecoder *self = FR_MEDIA_DECODER(o);
  FrDecoder *decoder = FR_DECODER(o);

  if (avcodec_is_open(self->ctx)) {

    fr_media_decoder_close_i(decoder);
  }
  sys_object_unref(self->stream);
  sys_async_queue_clear_full(&self->queue);

  fr_media_decoder_free(self);

  SYS_OBJECT_CLASS(fr_media_decoder_parent_class)->dispose(o);
}

static void fr_media_decoder_class_init(FrMediaDecoderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrDecoderClass *dcls = FR_DECODER_CLASS(cls);

  cls->decode_frame = fr_media_decoder_decode_frame_i;
  cls->read = fr_media_decoder_read_i;
  cls->write = fr_media_decoder_write_i;

  dcls->open = fr_media_decoder_open_i;
  dcls->close = fr_media_decoder_close_i;

  ocls->dispose = fr_media_decoder_dispose;
}

void fr_media_decoder_init(FrMediaDecoder* self) {

  sys_async_queue_init_full(&self->queue, (SysDestroyFunc)_sys_object_unref);
}
