#include <Framework/Media/FrMedia.h>

const SysChar* fr_media_error_string(SysInt err) {
  const SysChar* qmsg;
  qmsg = av_err2str(err);
  return sys_quark_string(qmsg);
}

SysBool fr_media_check_error(SysInt err, const SysChar* msg) {
  if (err == FR_MEDIA_STATE_EAGAIN 
    || err == FR_MEDIA_STATE_EOF
    || err >= 0) {

    return true;
  }

  sys_warning_N("errror in thread: %s, %s",
    msg, av_err2str(err));

  return false;
}

static SysInt fr_media_convert_frame(struct SwsContext* sws_ctx,
  AVFrame* src, AVFrame* dst) {
  sys_return_val_if_fail(sws_ctx != NULL, -1);
  sys_return_val_if_fail(src != NULL, -1);

  return sws_scale(sws_ctx,
    (const uint8_t* const*)src->data,
    src->linesize,
    0,
    src->height,
    dst->data,
    dst->linesize);
}

static AVFrame* fr_media_new_agba_frame(struct SwsContext* sws_ctx, AVFrame* frame) {
  SysInt err;
  AVFrame* rgba_frame;

  rgba_frame = av_frame_alloc();
  rgba_frame->format = AV_PIX_FMT_ARGB;
  rgba_frame->width = frame->width;
  rgba_frame->height = frame->height;

  err = av_frame_get_buffer(rgba_frame, 0);
  if (err < 0) {
    av_frame_free(&rgba_frame);
    return NULL;
  }

  fr_media_convert_frame(sws_ctx, frame, rgba_frame);
  return rgba_frame;
}

SysBool fr_media_to_frame(struct SwsContext* sws_ctx, AVFrame** frame) {
  sys_return_val_if_fail(sws_ctx != NULL, false);
  sys_return_val_if_fail(frame != NULL, false);
  sys_return_val_if_fail(*frame != NULL, false);

  AVFrame* rgba_frame;

  rgba_frame = fr_media_new_agba_frame(sws_ctx, *frame);
  if (rgba_frame == NULL) { return false; }
  av_frame_free(frame);
  *frame = rgba_frame;

  return true;
}

SysBool fr_media_data_save_to_png(SysInt width,
  SysInt height,
  SysUInt8* data,
  SysInt linesize,
  const SysChar *filename) {
/**
 * save_avframe_png: visual studio should set /MD for libpng
 * @frame: frame
 * @filename:
 *
 * Returns: void
 */
  FILE *fp = fopen(filename, "wb");
  if (!fp) { return false; }

  png_struct* png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png_ptr) { 
    fclose(fp);
    return false;
  }

  png_info* info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr) {
    png_destroy_write_struct(&png_ptr, NULL);
    fclose(fp);
    return false;
  }

  png_init_io(png_ptr, fp);
  png_set_IHDR(png_ptr,
      info_ptr,
      width,
      height,
      8, 
      PNG_COLOR_TYPE_RGB_ALPHA,
      PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_DEFAULT,
      PNG_FILTER_TYPE_DEFAULT);

  png_byte** bptr = (png_byte **)sys_malloc_N(sizeof(png_byte *) * height);
  for(int i = 0; i < height; i++) {
    bptr[i] = (png_byte *)(data + i * linesize);
  }

  png_set_rows(png_ptr, info_ptr, bptr);
  png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
  sys_free_N(bptr);

  png_destroy_write_struct(&png_ptr, &info_ptr);
  fclose(fp);
  return true;
}

void fr_media_rgba_save_to_png(AVFrame* frame, const SysChar* filename) {

  fr_media_data_save_to_png(frame->width, frame->height, frame->data[0], frame->linesize[0], filename);
}


void fr_media_yuv_save_to_png(AVFrame *src, const SysChar *filename) {
  struct SwsContext *sws_ctx;
  AVFrame* rgba_frame;

  sws_ctx = sws_getContext(
      src->width, src->height, src->format,
      src->width, src->height, AV_PIX_FMT_RGBA,
      SWS_BILINEAR, NULL, NULL, NULL);

  rgba_frame = fr_media_new_agba_frame(sws_ctx, src);
  if (rgba_frame == NULL) { return; }

  fr_media_rgba_save_to_png(rgba_frame, filename);
  av_frame_free(&rgba_frame);
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

SysInt fr_media_scale_frame(struct SwsContext *sws_ctx,
    AVFrame *frame,
    SysUInt8 * const pixels[],
    const SysInt pixels_size[]) {

  return sws_scale(sws_ctx,
      (const uint8_t *const *)frame->data,
      frame->linesize,
      0,
      frame->height,
      pixels,
      pixels_size);
}

SysInt fr_media_read_packet(AVFormatContext *ctx, AVPacket *p) {
  sys_return_val_if_fail(ctx != NULL, -1);
  sys_return_val_if_fail(p != NULL, -1);
  SysInt err;

  err = av_read_frame(ctx, p);
  if(err < 0) {
    av_packet_unref(p);

    if(err == AVERROR_EOF) {
      return err;
    }

    sys_warning_N("error reading packet: %s", av_err2str(err));
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
    case AVMEDIA_TYPE_VIDEO:
      return frame->best_effort_timestamp;
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

  do {
    err = avcodec_receive_frame(codec, frame);

    if(err >= 0) {
      frame->pts = auto_pts == -1
        ? fr_media_frame_get_pts(frame, codec)
        : frame->pkt_dts;

      return err;
    }

    if (err == AVERROR_EOF) {
      return err;
    }

    // sys_warning_N("%s", av_err2str(err));
  } while (err != AVERROR(EAGAIN));

  return err;
}

SysInt fr_media_avcodec_try_send_packet(
    AVCodecContext* codec,
    AVPacket* pkt) {
  SysInt err;

  do {
    err = avcodec_send_packet(codec, pkt);
    if (err >= 0) {
      return err;
    }

    if (err == AVERROR_EOF) {
      return err;
    }

    sys_warning_N("%s", av_err2str(err));
  } while (err != AVERROR(EAGAIN));

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
