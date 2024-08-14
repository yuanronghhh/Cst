#include <Framework/Media/FrMedia.h>
#include <Framework/Media/FrVideoStream.h>
#include <Framework/Media/FrAudioStream.h>
#include <Framework/Media/FrAudioDecoder.h>
#include <Framework/Media/FrImageScale.h>
#include <Framework/Media/FrMediaFile.h>
#include <Framework/Media/FrMediaStream.h>
#include <Framework/Media/FrMediaDecoder.h>
#include <Framework/Media/FrHwAccel.h>
#include <Framework/Media/FrImageSaver.h>
#include <Framework/Media/FrMediaFrame.h>
#include <Framework/Media/FrVideoFrame.h>
#include <Framework/Media/FrAudioFrame.h>
#include <Framework/Graph/FrImage.h>
#include <Framework/Device/FrAudioDevice.h>

static enum AVPixelFormat hw_pix_format = 0;

SysInt fr_image_context_fill_buffer(FrImageContext *info) {
    sys_return_val_if_fail(info != NULL, -1);
    sys_return_val_if_fail(info->data_size > 0, -1);
    sys_return_val_if_fail(info->format >= 0, -1);
    sys_return_val_if_fail(info->width > 0, -1);
    sys_return_val_if_fail(info->height > 0, -1);

    info->data_size = av_image_alloc(
        info->nbuf,
        info->stride,
        info->width,
        info->height,
        info->format,
        1);
    info->data = info->nbuf[0];

    return info->data_size;
}

const SysChar* fr_media_error_string(SysInt err) {
  const SysChar* qmsg = NULL;
  switch (err) {
  case FR_MEDIA_ERROR_SUCCESS:
    qmsg = "success";
    break;
  case FR_MEDIA_ERROR_AGAIN:
    qmsg = "again";
    break;
  case FR_MEDIA_ERROR_EINVAL:
    qmsg = "einval";
    break;
  case FR_MEDIA_ERROR_EOF:
    qmsg = "eof";
    break;
  case FR_MEDIA_ERROR_UNKNOWN:
    qmsg = "unknown";
    break;
  default:
    qmsg = av_err2str(err);
    break;
  }

  return qmsg;
}

static SysInt error_check_msg(SysInt err, const SysChar *msg) {
  if(err >= 0) {
    return err;
  }

  if (err == FR_MEDIA_ERROR_AGAIN
      || err == FR_MEDIA_ERROR_EOF) {

  } else {

    sys_warning_N("%s: %s", av_err2str(err), msg);
  }

  return err;
}

static SysInt error_check(SysInt err) {
  if(err >= 0) {
    return err;
  }

  if (err == FR_MEDIA_ERROR_AGAIN 
      || err == FR_MEDIA_ERROR_EOF) {

  } else {

    sys_warning_N("%d,%s", err, av_err2str(err));
  }

  return err;
}

static SysInt media_hwframe_transfer_data(AVFrame *dst, AVFrame *src) {
  SysInt err = av_hwframe_transfer_data(dst, src, 0);
  return error_check_msg(err, "hardware transfer data failed");
}

static SysType media_get_stream_type_by_index(FR_MEDIA_ENUM idx) {
  SysType tp = 0;
  switch(idx) {
    case FR_MEDIA_AUDIO:
      tp = FR_TYPE_AUDIO_STREAM;
      break;
    case FR_MEDIA_VIDEO:
      tp = FR_TYPE_VIDEO_STREAM;
      break;
    default:
      break;
  }

  return tp;
}

static AVFormatContext* media_create_context_by_filename(
    const SysChar* default_dec,
    const SysChar* filename) {
  int err;
  const AVInputFormat* default_format;
  AVFormatContext* ctx = avformat_alloc_context();

  default_format = av_find_input_format(default_dec);
  err = avformat_open_input(
      &ctx,
      filename,
      (AVInputFormat *)default_format,
      NULL);
  if (err < 0) {
    sys_warning_N("avformat_open_input: %s, %s", av_err2str(err), filename);
    return NULL;
  }

  err = avformat_find_stream_info(ctx, NULL);
  if (err < 0) {
    sys_warning_N("avformat_find_stream_info: %s, %s",
        av_err2str(err), filename);
    goto fail;
  }

  return ctx;

fail:
  avformat_free_context(ctx);
  return NULL;
}

SysInt fr_media_audio_open(FrAudioDecoder *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return -1;
}

SysBool fr_media_stream_get_rational(FrMediaStream* self, FrRational* rt) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(rt != NULL, false);
  AVStream *stream = self->ctx;

  *rt = stream->r_frame_rate;
  return true;
}

static SysBool format_context_is_realtime(AVFormatContext* s) {
  if (!strcmp(s->iformat->name, "rtp")
      || !strcmp(s->iformat->name, "rtsp")
      || !strcmp(s->iformat->name, "sdp"))
  {
    return 1;
  }

  if (s->pb
      && (!strncmp(s->url, "rtp:", 4) || !strncmp(s->url, "udp:", 4)))
  {
    return true;
  }

  return false;
}

static AVStream* media_parse_stream_by_type(
    AVFormatContext* ctx,
    FR_MEDIA_ENUM mediaType) {
  sys_return_val_if_fail(ctx != NULL, NULL);

  AVStream* as;
  SysInt idx;

  idx = av_find_best_stream(ctx, (SysInt)mediaType,
      -1, -1, NULL, 0);
  if (idx < 0) { return NULL; }

  as = ctx->streams[idx];

  return as;
}

static FrMediaStream* parse_stream_by_type(
    AVFormatContext *ctx,
    FR_MEDIA_ENUM mediaType) {
  sys_return_val_if_fail(ctx != NULL, NULL);

  FrMediaStream *stream;
  AVStream *as;
  SysType tp;

  as = media_parse_stream_by_type(ctx, mediaType);
  if(as == NULL) { return NULL; }
  tp = media_get_stream_type_by_index(mediaType);

  FrMediaStreamContext info = { .ctx = as };
  stream = sys_object_new(tp, NULL);
  fr_media_stream_construct(stream, &info);

  return stream;
}

void fr_media_stream_create(FrMediaStream *self,
    FrMediaStreamContext *info) {

  sys_assert(info->ctx != NULL);
  self->ctx = info->ctx;
}

void fr_media_stream_free(FrMediaStream *self) {
  // avformat_free_context to free
}

static const AVOutputFormat *media_find_audio_device(void) {
  const AVOutputFormat *format = NULL;

#if LIBAV_DEVICE
  format = av_output_audio_device_next(NULL);
  while(format == NULL) {

    format = av_output_audio_device_next(format);
  }

  if (format == NULL)
    format = av_output_audio_device_next (NULL);
#endif

  return format;
}

static AVFrame* new_rgba_frame(
    SysInt width,
    SysInt height,
    SysInt format,
    SysBool withbuf) {

  SysInt err;
  AVFrame* rgba_frame;

  rgba_frame = av_frame_alloc();
  rgba_frame->format = format;
  rgba_frame->width = width;
  rgba_frame->height = height;

  if(withbuf) {
    err = av_frame_get_buffer(rgba_frame, 0);

    if (err < 0) {
      sys_warning_N("new rgba frame failed, check width: %d,%d", width, height);
      av_frame_free(&rgba_frame);
      return NULL;
    }
  }

  return rgba_frame;
}

SysInt fr_media_image_scale_scale(
    FrImageScale *self,
    const uint8_t *const src_data[],
    const int src_stride[],
    int src_y, 
    int src_h,
    uint8_t *const dst_data[],
    const int dst_stride[]) {

  return sws_scale(self->ctx,
      src_data,
      src_stride,
      0,
      src_h,
      dst_data,
      dst_stride);
}

static SysInt image_scale_scale_avframe(
    FrImageScale *self,
    AVFrame *src,
    AVFrame *dst) {
  sys_return_val_if_fail(src != NULL, -1);
  sys_return_val_if_fail(dst != NULL, -1);
  SysInt err;

  if(!fr_image_scale_check(self, src->format, dst->format)) {
    return -1;
  }

  err =  fr_media_image_scale_scale(self,
      (const uint8_t *const *)src->data,
      src->linesize,
      0,
      src->height,
      dst->data,
      dst->linesize);

  return err;
}

static SysBool media_scale_hw_check(
    FrImageScale *self,
    AVFrame *src) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(src != NULL, false);
  const AVPixFmtDescriptor *desc;

  desc = av_pix_fmt_desc_get(src->format);
  if(desc == NULL) { return false; }

  if (!(desc->flags & AV_PIX_FMT_FLAG_HWACCEL)) {
    return true;
  }

  return true;
}

SysBool fr_media_scale_copy_gpu_frame(FrImageScale *scale, FrMediaFrame *frame) {
  AVFrame* nframe = av_frame_alloc();
  if (nframe == NULL) { return false; }

  if(!media_scale_hw_check(scale, frame->ctx)) {

    goto done;
  }

  if(media_hwframe_transfer_data(nframe, frame->ctx) < 0) {

    goto done;
  }

  fr_media_frame_free(frame);
  frame->ctx = nframe;

  return true;
done:
  av_frame_free(&nframe);
  return false;
}

SysBool fr_media_scale_media_frame(FrImageScale *scale, FrMediaFrame *frame) {
  sys_return_val_if_fail(frame != NULL, false);
  sys_return_val_if_fail(scale != NULL, false);

  AVFrame* nframe = new_rgba_frame(
      scale->out_width,
      scale->out_height,
      scale->out_pix_fmt,
      true);
  if (nframe == NULL) { return false; }

  if (image_scale_scale_avframe(scale, frame->ctx, nframe) < 0) {
    sys_warning_N("convert avframe failed: %p", scale);
    av_frame_free(&nframe);
    return false;
  }
  fr_media_frame_free(frame);
  frame->ctx = nframe;

  return 0;
}

void fr_media_rgba_save_to_png(AVFrame* frame, const SysChar* filename) {
  FrImageSaver *saver;
  FrImage *src;

  saver = fr_image_saver_new_I();
  src = fr_image_new_from_avframe(frame);

  fr_image_saver_save_png(saver, src, filename);

  sys_object_unref(saver);
  sys_object_unref(src);
}

static void fr_media_yuv_save_to_png(AVFrame *frame, const SysChar *filename) {
  FrImageScaleContext scale_info = {
    .in_width = frame->width,
    .in_height = frame->height,
    .in_pix_fmt = frame->format,
    .out_width = frame->width,
    .out_height = frame->height,
    .out_pix_fmt = AV_PIX_FMT_RGBA
  };

  FrImageSaver *saver;
  FrImageScale *scale;
  FrImage *image;

  scale = fr_image_scale_new_I(&scale_info);
  saver = fr_image_saver_new_I();
  image = fr_image_new_from_avframe(frame);

  fr_image_scale_scale_format(scale, image, scale_info.out_pix_fmt);
  fr_image_saver_save_png(saver, image, filename);

  sys_object_unref(saver);
  sys_object_unref(scale);
  sys_object_unref(image);
}

void fr_media_media_frame_init(FrMediaFrame* self, FrMediaStream *stream) {
  AVFrame *frame = self->ctx;
  AVStream *astream = stream->ctx;

  self->timestamp = av_rescale_q (frame->pts,
      astream->time_base,
      AV_TIME_BASE_Q);
}

void fr_media_video_frame_init(FrVideoFrame* self, FrMediaStream *stream) {
  sys_return_if_fail(self != NULL);

  AVFrame *frame = self->parent.ctx;

  self->width = frame->width;
  self->height = frame->height;
}

SysUInt8* fr_media_audio_frame_get_data(FrAudioFrame* self) {
  AVFrame *frame = self->parent.ctx;

  return frame->data[0];
}

void fr_media_audio_frame_init(FrAudioFrame* self, FrMediaStream *stream) {
  sys_return_if_fail(self != NULL);

  AVFrame *frame = self->parent.ctx;

  self->nb_samples = frame->nb_samples;
  self->sample_rate = frame->sample_rate;
  self->channels = frame->ch_layout.nb_channels;
}

void fr_media_frame_get_frame_rate (
    AVFormatContext *ctx,
    AVStream *stream,
    AVFrame *frame,
    SysInt *num,
    SysInt *den) {

  AVRational rational = av_guess_frame_rate(ctx, stream, frame);
  *num = rational.num;
  *den = rational.den;
}

static SysInt media_read_packet(AVFormatContext *ctx, AVPacket *p) {
  sys_return_val_if_fail(ctx != NULL, -1);
  sys_return_val_if_fail(p != NULL, -1);
  SysInt err;

  err = av_read_frame(ctx, p);
  if(err < 0) {
    av_packet_unref(p);

    // sys_warning_N("error reading packet: %s", fr_media_error_string(err));
  }

  return err;
}

SysInt fr_media_media_file_read_packet(FrMediaFile *self,
    FrMediaPacket *pkt) {
  sys_return_val_if_fail(self != NULL, -1);
  sys_return_val_if_fail(pkt != NULL, -1);

  return media_read_packet(self->ctx, pkt->ctx);
}

SysInt fr_media_packet_get_stream_index(FrMediaPacket *self) {
  sys_return_val_if_fail(self != NULL, -1);
  AVPacket *ctx = self->ctx;

  return ctx->stream_index;
}

void fr_media_packet_free(FrMediaPacket *self) {

  av_packet_free((AVPacket **)&self->ctx);
}

static AVCodecContext *media_create_avcodec_context(const AVCodec *codec,
    AVStream *stream) {

  sys_return_val_if_fail(stream != NULL, NULL);
  sys_return_val_if_fail(codec != NULL, NULL);
  AVCodecContext *avctx;
  SysInt err;

  avctx = avcodec_alloc_context3(codec);
  if(avctx == NULL) { return NULL; }

  err = avcodec_parameters_to_context(avctx, stream->codecpar);
  if (err < 0) { goto fail; }
  avctx->pkt_timebase = stream->time_base;
  avctx->skip_frame = stream->discard;

  return avctx;
fail:
  if(avctx != NULL) {

    avcodec_free_context(&avctx);
  }

  return NULL;
}

static const AVCodec *media_find_decoder(AVStream *stream) {
  sys_return_val_if_fail(stream != NULL, NULL);

  return avcodec_find_decoder(stream->codecpar->codec_id);
}

void fr_media_decoder_create(FrMediaDecoder *self,
    FrMediaDecoderContext *info) {
  AVStream *as = info->media_stream->ctx;

  self->codec = (SysPointer)media_find_decoder(as);
  self->ctx = (SysPointer)media_create_avcodec_context(self->codec, as);
}

void fr_media_decoder_get_info(FrMediaDecoder *o, 
    SysInt *width,
    SysInt *height,
    SysInt *pix_fmt) {

  AVCodecContext *ctx = o->ctx;

  *width = ctx->width;
  *height = ctx->height;
  *pix_fmt = ctx->pix_fmt;
}

void fr_media_decoder_free(FrMediaDecoder *self) {

  avcodec_free_context((AVCodecContext **)&self->ctx);
}

static AVPacket* media_packet_new_from_avpacket(AVPacket* op) {
  sys_return_val_if_fail(op != NULL, NULL);

  SysInt err;

  AVPacket* np = av_packet_alloc();

  if (!np) {
    return NULL;

  }
  else if (np->data) {

    err = av_packet_ref(np, op);
  }
  else {

    err = av_new_packet(np, op->size);
  }

  if (err < 0) {

    av_packet_free(&np);
    return NULL;
  }

  return np;
}

static SysInt media_avcodec_try_receive_frame (
    AVCodecContext *codec,
    AVFrame *frame) {
  SysInt err;

  err = avcodec_receive_frame(codec, frame);
  return error_check(err);
}

SysInt fr_media_decoder_receive_frame(
    FrMediaDecoder* self,
    FrMediaFrame **nframe) {

  sys_return_val_if_fail(self != NULL, -1);
  sys_return_val_if_fail(*nframe == NULL, -1);

  SysInt err;
  FrMediaFrame* frame = self->frame;

  err = media_avcodec_try_receive_frame(self->ctx,
      frame->ctx);

  if (err < 0) {
    if(err == FR_MEDIA_ERROR_EOF) {
      fr_decoder_set_eof(FR_DECODER(self), true);
      avcodec_flush_buffers(self->ctx);
    }

    return err;
  }

  fr_media_frame_init_frame(frame, self->stream);
  *nframe = frame;

  return err;
}

static SysInt media_avcodec_try_send_packet(
    AVCodecContext* codec,
    AVPacket* pkt) {
  SysInt err;

  err = avcodec_send_packet(codec, pkt);
  return error_check(err);
}

SysInt fr_media_decoder_send_packet(FrMediaDecoder* self,
    FrMediaPacket *pkt) {
  sys_return_val_if_fail(self != NULL, -1);

  return media_avcodec_try_send_packet(self->ctx, pkt->ctx);
}

SysBool fr_media_file_create(
    FrMediaFile *self,
    const SysChar *filename) {
  const SysChar* default_str = sys_path_extension(filename);
  AVFormatContext* ctx = media_create_context_by_filename(default_str,
      filename);
  FrMediaStream *stream;

  self->seek.seek_flags |= AVSEEK_FLAG_BYTE;
  self->is_realtime = format_context_is_realtime(ctx);

  self->n_streams = ctx->nb_streams;
  self->streams = (FrMediaStream **)sgc_type_new(SYS_TYPE_POINTER, 
      self->n_streams);

  stream = parse_stream_by_type(ctx, FR_MEDIA_VIDEO);
  if(stream) { self->streams[FR_MEDIA_VIDEO] = stream; }

  stream = parse_stream_by_type(ctx, FR_MEDIA_AUDIO);
  if(stream) { self->streams[FR_MEDIA_AUDIO] = stream; }

  stream = parse_stream_by_type(ctx, FR_MEDIA_SUBTITLE);
  if(stream) { self->streams[FR_MEDIA_SUBTITLE] = stream; }

  self->ctx = ctx;

  return true;
}

SysInt fr_hw_accel_get_hw_format(FrHwAccel *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return hw_pix_format;
}

static enum AVPixelFormat get_hw_format(
    AVCodecContext *ctx,
    const enum AVPixelFormat *pix_fmts) {
  const enum AVPixelFormat *p;

  for (p = pix_fmts; *p != -1; p++) {
    if (*p == hw_pix_format)
      return *p;
  }

  sys_warning_N("%s", "Failed to get HW surface format.");
  return AV_PIX_FMT_NONE;
}

void fr_hw_accel_free(FrHwAccel *self) {

  av_buffer_unref((AVBufferRef **)&self->ctx);
}

SysBool fr_hw_accel_create(FrHwAccel *self, FrHwAccelContext *info) {
  AVBufferRef *ctx = NULL;
  FrMediaDecoder *dec = info->decoder;

  SysInt type = av_hwdevice_find_type_by_name(info->name);
  if(type == 0) {

    sys_warning_N("not support hardware device %s", info->name);
    return false;
  }

  if(!fr_media_decoder_get_hw_info(dec, type, &hw_pix_format)) {
    sys_info_N("Failed to get hw info: %s, %s",
        fr_decoder_get_name(FR_DECODER(dec)),
        av_hwdevice_get_type_name(type));
    return false;
  }

  /* TODO: copy from example  */
  AVCodecContext *cctx = dec->ctx;
  cctx->get_format  = get_hw_format;
  if(av_hwdevice_ctx_create(&ctx, type, NULL, NULL, 0) < 0) {

    sys_warning_N("%s", "Failed to create specified HW device.");
    return false;
  }
  cctx->hw_device_ctx = av_buffer_ref(ctx);
  info->ctx = ctx;

  return true;
}

SysInt fr_media_frame_get_format(FrMediaFrame *self) {
  sys_return_val_if_fail(self != NULL, -1);
  AVFrame *ctx = self->ctx;

  return ctx->format;
}

void fr_media_frame_free(FrMediaFrame *self) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->ctx != NULL);

  av_frame_free((AVFrame **)&self->ctx);
}

void fr_media_frame_ref(FrMediaFrame *nself, FrMediaFrame *oself) {

  av_frame_ref(nself->ctx, oself->ctx);
}

SysInt64 fr_media_frame_get_pts(FrMediaFrame *self) {
  sys_return_val_if_fail(self != NULL, -1);
  AVFrame *avf = self->ctx;

  return avf->pts;
}

void fr_media_frame_get_linesize(FrMediaFrame *self, SysInt linesize[]) {
  AVFrame *avf = self->ctx;

  for(SysInt i = 0; i < AV_NUM_DATA_POINTERS; i++) {
    linesize[i] = avf->linesize[i];
  }
}

void fr_media_audio_stream_create(FrAudioStream *o,
    FrAudioStreamContext *info) {
}

void fr_audio_stream_get_device_info(FrAudioStream *self,
    FrAudioDeviceContext *info) {
  sys_return_if_fail(self != NULL);
  AVStream *avf = self->parent.ctx;

  /* AVSampleFormat */
  info->format = avf->codecpar->format;
  info->sample_rate = avf->codecpar->sample_rate;
  info->channels = avf->codecpar->ch_layout.nb_channels;
}

void fr_media_frame_get_info(FrMediaFrame *self,
    SysUInt8 *data[],
    SysInt linesize[]) {
  AVFrame *avf = self->ctx;

  for(SysInt i = 0; i < FR_MEDIA_NUM_DATA; i++) {
    linesize[i] = avf->linesize[i];
    data[i] = avf->data[i];
  }
}

void fr_media_frame_get_data(FrMediaFrame *self, SysUInt8 *data[]) {
  AVFrame *avf = self->ctx;

  for(SysInt i = 0; i < AV_NUM_DATA_POINTERS; i++) {
    data[i] = avf->data[i];
  }
}

AVFilter* fr_media_create_filter_context(void) {

  return NULL;
}
