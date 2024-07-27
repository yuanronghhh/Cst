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

void fr_media_decoder_create(FrMediaDecoder *self,
    FrMediaStream *stream);

SysInt64 fr_media_stream_calc_pts(
    FrMediaStream *self,
    FrMediaFrame *frame);

SysInt fr_media_decoder_receive_frame(
    FrMediaDecoder* self,
    FrMediaFrame **nframe);

SysInt fr_media_read_packet(AVFormatContext *ctx, AVPacket *p);

SysInt fr_media_decoder_send_packet(FrMediaDecoder* self,
    FrMediaPacket *pkt);

SysInt fr_media_image_scale_scale(
    FrImageScale *self,
    const uint8_t *const src_data[],
    const int src_stride[],
    int src_y, 
    int src_h,
    uint8_t *const dst_data[],
    const int dst_stride[]);

SysBool fr_media_file_create(FrMediaFile *self,
    const SysChar *filename);

void fr_media_frame_free(FrMediaFrame *self);
void fr_media_frame_get_data(FrMediaFrame *self, uint8_t *data[]);
void fr_media_frame_get_linesize(FrMediaFrame *self, SysInt linesize[]);
SysInt fr_media_frame_get_format(FrMediaFrame *self);

void fr_media_video_frame_init(FrVideoFrame* self, FrMediaStream *stream);
void fr_media_audio_frame_init(FrAudioFrame* self, FrMediaStream *stream);

SysBool fr_media_scale_media_frame(FrImageScale *scale, FrMediaFrame *frame);
SysBool fr_media_scale_copy_gpu_frame(FrImageScale *scale, FrMediaFrame *frame);

SYS_END_DECLS

#endif
