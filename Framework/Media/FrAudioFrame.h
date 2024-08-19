#ifndef __FR_AUDIO_FRAME_H__
#define __FR_AUDIO_FRAME_H__

#include <Framework/Media/FrMediaFrame.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AUDIO_FRAME (fr_audio_frame_get_type())
#define FR_AUDIO_FRAME(o) ((FrAudioFrame* )sys_object_cast_check(o, FR_TYPE_AUDIO_FRAME))
#define FR_AUDIO_FRAME_CLASS(o) ((FrAudioFrameClass *)sys_class_cast_check(o, FR_TYPE_AUDIO_FRAME))
#define FR_AUDIO_FRAME_GET_CLASS(o) sys_instance_get_class(o, FrAudioFrameClass)

struct _FrAudioFrameClass {
  FrMediaFrameClass parent;
};

struct _FrAudioFrame  {
  FrMediaFrame parent;

  /* <private> */
  SysInt format;
  SysInt sample_rate;
  SysInt channels;
};

SYS_API SysType fr_audio_frame_get_type(void);
SYS_API FrAudioFrame *fr_audio_frame_new(void);
#define fr_audio_frame_create(o) sys_object_create(o, FR_TYPE_AUDIO_FRAME)

SYS_API FrAudioFrame *fr_audio_frame_new_I(void);

SYS_END_DECLS

#endif
