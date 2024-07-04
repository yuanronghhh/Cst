#include <Framework/Media/FrMedia.h>
#include <Framework/Media/FrImageScale.h>
#include <Framework/Media/FrImageSaver.h>
#include <Framework/Graph/FrImage.h>

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

FR_MEDIA_ERROR_ENUM fr_media_error_map(SysInt err) {
  switch (err) {
    case 0:
      return FR_MEDIA_ERROR_SUCCESS;
    case AVERROR(EAGAIN):
      return FR_MEDIA_ERROR_AGAIN;
    case AVERROR_EOF:
      return FR_MEDIA_ERROR_EOF;
    case AVERROR(EINVAL):
      return FR_MEDIA_ERROR_EINVAL;
    default:
      sys_warning_N("decoder err not handle: %d", av_err2str(err));
      return FR_MEDIA_ERROR_UNKNOWN;
  }
}

AVFrame* fr_media_new_rgba_frame(
    SysInt width,
    SysInt height,
    SysInt format) {
  SysInt err;
  AVFrame* rgba_frame;

  rgba_frame = av_frame_alloc();
  rgba_frame->format = format;
  rgba_frame->width = width;
  rgba_frame->height = height;

  err = av_frame_get_buffer(rgba_frame, 0);
  if (err < 0) {
    sys_warning_N("new rgba frame failed, check width: %d,%d", width, height);
    av_frame_free(&rgba_frame);
    return NULL;
  }

  return rgba_frame;
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


void fr_media_yuv_save_to_png(AVFrame *frame, const SysChar *filename) {
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

  fr_image_scale_convert_format(scale, image, scale_info.out_pix_fmt);
  fr_image_saver_save_png(saver, image, filename);

  sys_object_unref(saver);
  sys_object_unref(scale);
  sys_object_unref(image);
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

SysInt fr_media_read_packet(AVFormatContext *ctx, AVPacket *p) {
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

AVCodecContext *fr_media_create_avcodec_context(const AVCodec *codec,
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

AVPacket* fr_media_packet_new_from_avpacket(AVPacket* op) {
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

const AVCodec *fr_media_find_decoder(AVStream *stream) {
  sys_return_val_if_fail(stream != NULL, NULL);

  return avcodec_find_decoder(stream->codecpar->codec_id);
}

SysInt64 fr_media_frame_get_audio_pts(AVFrame *frame,
    AVRational avctx_timebase) {
  sys_return_val_if_fail(frame != NULL, AV_NOPTS_VALUE);

  AVRational tb = (AVRational){1, frame->sample_rate};
  return av_rescale_q(frame->pts, avctx_timebase, tb);
}

SysInt64 fr_media_frame_get_pts(AVFrame *frame,
    AVCodecContext *avctx) {
  sys_return_val_if_fail(frame != NULL, AV_NOPTS_VALUE);
  sys_return_val_if_fail(avctx != NULL, AV_NOPTS_VALUE);

  switch(avctx->codec_type) {
    case AVMEDIA_TYPE_VIDEO: {
      return av_rescale_q (frame->best_effort_timestamp,
          avctx->time_base,
          (AVRational) { 1, 1e6 });
    }
    case AVMEDIA_TYPE_AUDIO: {
      AVRational tb = (AVRational){1, frame->sample_rate};
      return av_rescale_q(frame->pts, avctx->pkt_timebase, tb);
    }
    default:
      sys_warning_N("failed to get pts: %d", avctx->codec_type);
      break;
  }

  return AV_NOPTS_VALUE;
}

SysInt fr_media_avcodec_try_receive_frame (
    AVCodecContext *codec,
    AVFrame *frame,
    SysInt auto_pts) {
  SysInt err;

  err = avcodec_receive_frame(codec, frame);
  if(err >= 0) {
    frame->pts = fr_media_frame_get_pts(frame, codec);
    return err;
  }

  if (err == FR_MEDIA_ERROR_AGAIN 
      || err == FR_MEDIA_ERROR_EOF) {

  } else {

    sys_warning_N("%d,%s", err, av_err2str(err));
  }

  return err;
}

SysInt fr_media_avcodec_try_send_packet(
    AVCodecContext* codec,
    AVPacket* pkt) {
  SysInt err;

  err = avcodec_send_packet(codec, pkt);
  if (err >= 0) {
    return err;
  }

  if(err != FR_MEDIA_ERROR_AGAIN
      && err != FR_MEDIA_ERROR_EOF) {

    sys_warning_N("error %s", av_err2str(err));
  }

  return err;
}

SysInt fr_media_avcodec_receive_frame (
    AVCodecContext *codec,
    AVFrame *frame,
    SysInt64 pts) {

  SysInt err;
  err = avcodec_receive_frame(codec, frame);
  frame->pts = pts;

  return err;
}

AVFormatContext* fr_media_create_context_by_filename(
    const SysChar* default_dec,
    const SysChar* filename) {
  int err;
  const AVInputFormat* default_format;
  AVFormatContext* ctx = NULL;

  default_format = av_find_input_format(default_dec);
  err = avformat_open_input(&ctx, filename, (AVInputFormat*)default_format, NULL);
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

AVStream* fr_media_parse_stream_by_type(
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

AVFilter* fr_media_create_filter_context(void) {

  return NULL;
}
