#ifndef __FR_AUDIO_DEVICE_H__
#define __FR_AUDIO_DEVICE_H__

#include <Framework/Device/FrDevice.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AUDIO_DEVICE (fr_audio_device_get_type())
#define FR_AUDIO_DEVICE(o) ((FrAudioDevice* )sys_object_cast_check(o, FR_TYPE_AUDIO_DEVICE))
#define FR_AUDIO_DEVICE_CLASS(o) ((FrAudioDeviceClass *)sys_class_cast_check(o, FR_TYPE_AUDIO_DEVICE))
#define FR_AUDIO_DEVICE_GET_CLASS(o) sys_instance_get_class(o, FrAudioDeviceClass)

struct _FrAudioDeviceClass {
  FrDeviceClass parent;
};

struct _FrAudioDevice {
  FrDevice parent;

  /* <private> */
  /* SDL_AudioDevice */
  SysPointer ctx;

  SysInt channels;
  SysInt sample_rate;
  SysInt format;
};

struct _FrAudioDeviceContext {
  SysInt channels;
  SysInt sample_rate;
  SysInt format;
};

SYS_API SysType fr_audio_device_get_type(void);
SYS_API FrDevice *fr_audio_device_new(void);
SYS_API FrDevice *fr_audio_device_new_I(FrAudioDeviceContext *info);
SYS_API FrDevice *fr_audio_device_find_by_id(SysUInt deviceID);
SYS_API FrDevice *fr_audio_device_find_by_media_file(FrMediaFile *file);
SYS_API void fr_audio_device_get_info(FrAudioDevice *self,
  SysInt *channels,
  SysInt *sample_rate,
  SysInt *format);

SYS_API void fr_audio_device_resume(FrAudioDevice *dev);

SYS_END_DECLS

#endif
