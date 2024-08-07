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

void fr_media_stream_create(FrMediaStream *self,
    FrMediaStreamContext *info);

void fr_media_stream_free(FrMediaStream *self);

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

/* image */
SysInt fr_image_context_fill_buffer(FrImageContext *info);
#define fr_image_get_size(format, width, height) av_image_get_buffer_size(format, width, height, 1)

SysInt fr_hw_accel_get_hw_format(FrHwAccel *self);
void fr_hw_accel_free(FrHwAccel *self);
SysBool fr_hw_accel_create(FrHwAccel *self, FrHwAccelContext *info);

SysBool fr_media_stream_get_rational(FrMediaStream* self, FrRational* rt);
void fr_media_frame_free(FrMediaFrame *self);
void fr_media_frame_ref(FrMediaFrame *nself, FrMediaFrame *oself);
void fr_media_frame_get_data(FrMediaFrame *self, uint8_t *data[]);
void fr_media_frame_get_linesize(FrMediaFrame *self, SysInt linesize[]);
SysInt fr_media_frame_get_format(FrMediaFrame *self);
SysInt64 fr_media_frame_get_pts(FrMediaFrame *self);

void fr_media_media_frame_init(FrMediaFrame* self, FrMediaStream *stream);
void fr_media_video_frame_init(FrVideoFrame* self, FrMediaStream *stream);
void fr_media_audio_frame_init(FrAudioFrame* self, FrMediaStream *stream);

SysBool fr_media_scale_media_frame(FrImageScale *scale, FrMediaFrame *frame);
SysBool fr_media_scale_copy_gpu_frame(FrImageScale *scale, FrMediaFrame *frame);

SYS_END_DECLS

#endif
