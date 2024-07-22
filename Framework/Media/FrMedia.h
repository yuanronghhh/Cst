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

SysInt fr_media_avcodec_try_receive_frame (
    AVCodecContext *codec,
    AVFrame *frame);

SysInt fr_media_avcodec_try_send_packet(
  AVCodecContext* codec,
  AVPacket* pkt);

AVFormatContext* fr_media_create_context_by_filename(
  const SysChar* default_dec,
  const SysChar* filename);

AVStream* fr_media_parse_stream_by_type(
  AVFormatContext* ctx,
  FR_MEDIA_ENUM mediaType);

SysInt fr_media_read_packet(AVFormatContext *ctx, AVPacket *p);

SysInt fr_media_image_scale_scale(
    FrImageScale *self,
    const uint8_t *const src_data[],
    const int src_stride[],
    int src_y, 
    int src_h,
    uint8_t *const dst_data[],
    const int dst_stride[]);

void fr_media_video_frame_init(FrVideoFrame* self);
SysBool fr_media_scale_media_frame(FrImageScale *scale, FrMediaFrame *frame);
SysBool fr_media_scale_copy_gpu_frame(FrImageScale *scale, FrMediaFrame *frame);

SYS_END_DECLS

#endif
