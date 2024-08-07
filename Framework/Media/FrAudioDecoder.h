#ifndef __FR_AUDIO_DECODER_H__
#define __FR_AUDIO_DECODER_H__

#include <Framework/Media/FrMediaDecoder.h>
#include <Framework/Media/FrAudioFrame.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AUDIO_DECODER (fr_audio_decoder_get_type())
#define FR_AUDIO_DECODER(o) ((FrAudioDecoder* )sys_object_cast_check(o, FR_TYPE_AUDIO_DECODER))
#define FR_AUDIO_DECODER_CLASS(o) ((FrAudioDecoderClass *)sys_class_cast_check(o, FR_TYPE_AUDIO_DECODER))
#define FR_AUDIO_DECODER_GET_CLASS(o) sys_instance_get_class(o, FrAudioDecoderClass)

struct _FrAudioDecoderClass {
  FrMediaDecoderClass parent;
};

struct _FrAudioDecoder {
  FrMediaDecoder parent;

  /* <private> */
};

struct _FrAudioDecoderContext {

  SysInt channels;
};

SYS_API SysType fr_audio_decoder_get_type(void);
SYS_API FrDecoder *fr_audio_decoder_new(void);

SYS_API FrDecoder *fr_audio_decoder_new_I(FrAudioDecoderContext *info);

SYS_END_DECLS

#endif
