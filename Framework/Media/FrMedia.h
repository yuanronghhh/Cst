#ifndef __FR_MEDIA_H__
#define __FR_MEDIA_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

const SysChar* fr_media_error_string(SysInt err);

void fr_media_frame_get_frame_rate (
    AVFormatContext *ctx,
    AVStream *stream,
    AVFrame *frame,
    SysInt *num,
    SysInt *den);

const AVCodec *fr_media_find_decoder(AVStream *stream);

AVCodecContext *fr_media_create_avcodec_context(const AVCodec *codec,
    AVStream *stream);

AVPacket* fr_media_packet_new_from_avpacket(AVPacket* op);

SysInt fr_media_avcodec_receive_frame (
    AVCodecContext *codec,
    AVFrame *frame,
    SysInt64 pts);

SysInt fr_media_avcodec_try_receive_frame (
    AVCodecContext *codec,
    AVFrame *frame,
    SysInt auto_pts);

SysInt fr_media_avcodec_try_send_packet(
  AVCodecContext* codec,
  AVPacket* pkt);

AVFormatContext* fr_media_create_context_by_filename(
  const SysChar* default_dec,
  const SysChar* filename);

SysInt64 fr_media_frame_get_pts(AVFrame* frame,
  AVCodecContext* avctx);

AVStream* fr_media_parse_stream_by_type(
  AVFormatContext* ctx,
  FR_MEDIA_ENUM mediaType);

SysInt fr_media_read_packet(AVFormatContext *ctx, AVPacket *p);

AVFrame* fr_media_new_rgba_frame(
    SysInt width,
    SysInt height,
    SysInt format);

SysInt fr_media_avframe_convert(
    FrImageScale *scale,
    AVFrame *frame,
    AVFrame *dst);

void fr_media_yuv_save_to_png(AVFrame * frame, const SysChar *filename);
void fr_media_rgba_save_to_png(AVFrame* frame, const SysChar* filename);

SYS_END_DECLS

#endif
