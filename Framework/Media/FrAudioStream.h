#ifndef __FR_AUDIO_STREAM_H__
#define __FR_AUDIO_STREAM_H__

#include <Framework/Media/FrMediaStream.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AUDIO_STREAM (fr_audio_stream_get_type())
#define FR_AUDIO_STREAM(o) ((FrAudioStream* )sys_object_cast_check(o, FR_TYPE_AUDIO_STREAM))
#define FR_AUDIO_STREAM_CLASS(o) ((FrAudioStreamClass *)sys_class_cast_check(o, FR_TYPE_AUDIO_STREAM))
#define FR_AUDIO_STREAM_GET_CLASS(o) sys_instance_get_class(o, FrAudioStreamClass)

struct _FrAudioStreamClass {
  FrMediaStreamClass parent;
};

struct _FrAudioStream {
  FrMediaStream parent;

  /* <private> */

  /* SDL_AudioStream */
  SysPointer ctx;
};

struct _FrAudioStreamContext {
  SysInt format;
  SysInt sample_rate;
  SysInt channels;
  SysBool is_in_stream;

  /* <private> */
  FrAudioDevice* dev;
};

SYS_API SysType fr_audio_stream_get_type(void);
SYS_API FrMediaStream *fr_audio_stream_new(void);

FrMediaStream *fr_audio_stream_new_I(FrAudioStreamContext *info);
FrMediaStream *fr_audio_stream_new_out_by_device(FrAudioDevice *dev);

SYS_END_DECLS

#endif
