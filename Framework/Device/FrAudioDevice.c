#include <Framework/Device/FrAudioDevice.h>
#include <Framework/Device/FrWindow.h>

SYS_DEFINE_TYPE(FrAudioDevice, fr_audio_device, FR_TYPE_DEVICE);


void fr_audio_device_resume(FrAudioDevice *dev) {

  fr_window_audio_resume(dev);
}

/* object api */
static void fr_audio_device_construct_i(FrDevice *self,
    FrAudioDeviceContext *info) {
  FrAudioDevice *dev = FR_AUDIO_DEVICE(self);

  fr_window_audio_open(dev, info);
}

FrDevice* fr_audio_device_new(void) {
  return sys_object_new(FR_TYPE_AUDIO_DEVICE, NULL);
}

FrDevice *fr_audio_device_new_I(FrAudioDeviceContext *info) {
  FrDevice *o = fr_audio_device_new();

  fr_audio_device_construct_i(o, info);

  return o;
}

static void fr_audio_device_dispose(SysObject* o) {
  FrAudioDevice *self = FR_AUDIO_DEVICE(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(fr_audio_device_parent_class)->dispose(o);
}

static void fr_audio_device_class_init(FrAudioDeviceClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_audio_device_dispose;
}

static void fr_audio_device_init(FrAudioDevice* self) {
}
