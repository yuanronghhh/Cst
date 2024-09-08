#ifndef __FR_MEDIA_H__
#define __FR_MEDIA_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_MEDIA_NUM_DATA AV_NUM_DATA_POINTERS

const SysChar* fr_media_error_string(SysInt err);

void fr_media_frame_get_frame_rate (
    AVFormatContext *ctx,
    AVStream *stream,
    AVFrame *frame,
    SysInt *num,
    SysInt *den);

const AVCodec *fr_media_find_decoder(AVStream *stream);

void fr_media_decoder_get_info(FrMediaDecoder *o, 
    SysInt *width,
    SysInt *height,
    SysInt *pix_fmt);

SysInt fr_media_decoder_open(FrMediaDecoder* self);

void fr_media_decoder_flush(FrMediaDecoder* self);

void fr_media_decoder_create(FrMediaDecoder *self,
    FrMediaDecoderContext *info);

SysBool fr_media_decoder_is_open(FrMediaDecoder *self);

void fr_media_decoder_free(FrMediaDecoder *self);

SysInt64 fr_media_stream_calc_pts(
    FrMediaStream *self,
    FrMediaFrame *frame);

void fr_media_stream_create(FrMediaStream *self,
    FrMediaStreamContext *info);

void fr_media_stream_free(FrMediaStream *self);

SysInt fr_media_decoder_receive_frame(
    FrMediaDecoder* self,
    FrMediaFrame **nframe);

SysInt fr_media_decoder_send_packet(FrMediaDecoder* self,
    FrMediaPacket *pkt);

SysInt64 fr_media_gcd(SysInt64 a, SysInt64 b);

SysInt fr_media_image_scale_scale(
    FrImageScale *self,
    const uint8_t *const src_data[],
    const int src_stride[],
    int src_y, 
    int src_h,
    uint8_t *const dst_data[],
    const int dst_stride[]);

SysInt fr_media_file_pause(FrMediaFile* self);

SysInt fr_media_file_play(FrMediaFile* self);

SysBool fr_media_file_create(FrMediaFile *self,
    const SysChar *filename);

SysInt fr_media_file_seek(FrMediaFile *self, SysInt64 seek_target);

void fr_media_file_free(FrMediaFile *self);
const SysChar *fr_media_file_get_url(FrMediaFile *self);

SysInt fr_media_media_file_read_packet(FrMediaFile *self,
    FrMediaPacket **mpkt);

void fr_media_media_packet_create(FrMediaPacket *self);
void fr_media_media_packet_free(FrMediaPacket *self);
void fr_media_media_packet_ref(FrMediaPacket* nself, FrMediaPacket* oself);
void fr_media_media_packet_unref(FrMediaPacket* nself);
SysInt fr_media_packet_get_stream_index(FrMediaPacket *self);

/* image */
SysInt fr_image_context_fill_buffer(FrImageContext *info);
#define fr_image_get_size(format, width, height) \
  av_image_get_buffer_size(format, width, height, 1)


#define fr_image_copy_to_buffer av_image_copy_to_buffer

SysInt fr_hw_accel_get_hw_format(FrHwAccel *self);
void fr_hw_accel_free(FrHwAccel *self);
SysBool fr_hw_accel_create(FrHwAccel *self, FrHwAccelContext *info);

SysBool fr_media_stream_get_rational(FrMediaStream* self, FrRational* rt);
void fr_media_media_frame_create(FrMediaFrame *self);
void fr_media_media_frame_free(FrMediaFrame *self);
void fr_media_media_frame_ref(FrMediaFrame *nself, FrMediaFrame *oself);
void fr_media_media_frame_unref(FrMediaFrame *nself);
void fr_media_frame_get_data(FrMediaFrame *self, uint8_t *data[]);
void fr_media_frame_get_linesize(FrMediaFrame *self, SysInt linesize[]);

void fr_media_frame_get_info(FrMediaFrame *self,
    SysUInt8 *data[],
    SysInt linesize[]);

SysInt fr_media_frame_get_format(FrMediaFrame *self);
SysInt64 fr_media_frame_get_pts(FrMediaFrame *self);

void fr_media_audio_stream_create(FrAudioStream *o, FrAudioStreamContext *info);
void fr_audio_stream_get_device_info(FrAudioStream *self,
    FrAudioDeviceContext *info);

void fr_media_media_frame_init(FrMediaFrame* self, FrMediaStream *stream);
void fr_media_video_frame_init(FrVideoFrame* self, FrMediaStream *stream);
void fr_media_audio_frame_init(FrAudioFrame* self, FrMediaStream *stream);

SysBool fr_media_scale_media_frame(FrImageScale *scale, FrMediaFrame *frame);
SysBool fr_media_scale_copy_gpu_frame(FrImageScale *scale, FrMediaFrame *frame);

SYS_END_DECLS

#endif
