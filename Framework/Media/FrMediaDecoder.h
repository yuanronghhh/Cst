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

   SysInt (*decode_frame) (FrMediaDecoder* o, FrMediaFrame **frame);
   SysInt (*read) (FrMediaDecoder *stream, FrPacket **pkt);
   SysInt (*write) (FrMediaDecoder *stream, FrPacket *pkt);
};

struct _FrMediaDecoder {
  FrDecoder parent;

  /* <private> */
  FrMediaStream *stream;
  SysAsyncQueue queue;
  FrMediaFrame *frame;
  SysType frame_type;

  /* const AVCodec *codec */
  SysPointer codec;
  /* AVCodecContext * */
  SysPointer ctx;
};

struct _FrMediaDecoderContext {
  SysType type;
  const SysChar *name;
  FrMediaStream *media_stream;
};

SYS_API SysType fr_media_decoder_get_type(void);
FrDecoder* fr_media_decoder_new(void);
void fr_media_decoder_set_type(SysType type);

SYS_API FrMediaStream *fr_media_decoder_get_stream(FrMediaDecoder *self);

SYS_API FrDecoder* fr_media_decoder_create_by_media_type(
    FrMediaFile* file,
    FR_MEDIA_ENUM mediaType);

SYS_API SysType fr_media_decoder_enum_to_type(FR_MEDIA_ENUM mediaType);

FrMediaDecoder* fr_media_decoder_new_by_type(
    SysType tp,
    const SysChar *name,
    FrMediaStream *ms);

SysInt fr_media_decoder_try_decode_frame(
    FrMediaDecoder* self,
    FrMediaFrame **nframe);

SysInt fr_media_decoder_receive_frame(
    FrMediaDecoder* self,
    FrMediaFrame **nframe);

SysInt fr_media_decoder_read(FrMediaDecoder *self, FrPacket **pkt);
void fr_media_decoder_write(FrMediaDecoder *self, FrPacket *pkt);

void fr_media_decoder_set_frame_type(FrMediaDecoder* self, SysType tp);

void fr_media_decoder_set_frame(FrMediaDecoder *self, FrMediaFrame * frame);
FrMediaFrame * fr_media_decoder_get_frame(FrMediaDecoder *self);

SYS_END_DECLS

#endif
