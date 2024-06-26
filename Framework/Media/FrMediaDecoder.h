#ifndef __FR_MEDIA_DECODER_H__
#define __FR_MEDIA_DECODER_H__

#include <Framework/Media/FrDecoder.h>
#include <Framework/Media/FrMediaFrame.h>

SYS_BEGIN_DECLS

#define FR_TYPE_MEDIA_DECODER (fr_media_decoder_get_type())
#define FR_MEDIA_DECODER(o) ((FrMediaDecoder* )sys_object_cast_check(o, FR_TYPE_MEDIA_DECODER))
#define FR_MEDIA_DECODER_CLASS(o) ((FrMediaDecoderClass *)sys_class_cast_check(o, FR_TYPE_MEDIA_DECODER))
#define FR_MEDIA_DECODER_GET_CLASS(o) sys_instance_get_class(o, FrMediaDecoderClass)

struct _FrMediaDecoderClass {
  FrDecoderClass parent;

  void (*construct) (
    FrMediaDecoder *o,
    FrDecoderContext *info,
    FrMediaStream *ms);

   FrMediaFrame *(*get_frame) (FrMediaDecoder* o);
   SysInt (*decode_frame) (FrMediaDecoder* o, FrMediaFrame **frame);
};

struct _FrMediaDecoder {
  FrDecoder parent;

  /* <private> */
  FrMediaStream *stream;
  const AVCodec *codec;
  AVCodecContext *ctx;
  // pkt_timebase{ num = 100 den = 2997 }	AVRational
  FrMediaFrame *frame;
  SysInt auto_pts;
};

SYS_API SysType fr_media_decoder_get_type(void);
FrDecoder* fr_media_decoder_new(void);

SYS_API FrDecoder *fr_media_decoder_create_by_media_type(FrMediaFile *file,
    FR_MEDIA_ENUM mediaType);

SYS_API SysType fr_media_decoder_enum_to_type(FR_MEDIA_ENUM mediaType);
void fr_media_decoder_flush(FrMediaDecoder* self);

SysInt fr_media_decoder_send_packet(FrMediaDecoder* self,
    FrMediaPacket *pkt);

FrDecoder* fr_media_decoder_new_by_type(
    SysType tp,
    const SysChar *name,
    FrMediaStream *ms);

void fr_media_decoder_construct(FrMediaDecoder *self,
    FrDecoderContext *info, 
    FrMediaStream *ms);

SysInt fr_media_decoder_try_decode_frame(
    FrMediaDecoder* self,
    FrMediaFrame **nframe);

SysInt fr_media_decoder_receive_frame(
    FrMediaDecoder* self,
    FrMediaFrame **nframe);

SYS_END_DECLS

#endif
