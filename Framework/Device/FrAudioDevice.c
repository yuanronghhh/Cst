#include <Framework/Device/FrAudioDevice.h>
#include <Framework/Media/FrMediaFile.h>
#include <Framework/Device/FrWindow.h>

static SysHSList *g_devices = NULL;

SYS_DEFINE_TYPE(FrAudioDevice, fr_audio_device, FR_TYPE_DEVICE);

void fr_audio_device_resume(FrAudioDevice *dev) {

  fr_window_audio_resume(dev);
}

FrDevice *fr_audio_device_find_by_media_file(FrMediaFile *file) {
  sys_return_val_if_fail(file != NULL, NULL);
  FrAudioDevice *dev;

  if(!fr_media_file_has_audio(file)) { return NULL; }

  FrAudioDeviceContext info = {0};
  FrAudioStream *astream = fr_media_file_stream_by_type(file, FR_MEDIA_AUDIO);

  sys_hslist_foreach(g_devices, item) {
    dev = FR_AUDIO_DEVICE(item);
  }

  return dev;

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

  sys_return_val_if_fail(info != NULL, NULL);
  sys_return_val_if_fail(info->format != 0, NULL);
  sys_return_val_if_fail(info->channels != 0, NULL);
  sys_return_val_if_fail(info->sample_rate != 0, NULL);

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
