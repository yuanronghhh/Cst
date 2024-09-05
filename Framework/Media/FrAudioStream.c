#include <Framework/Media/FrAudioStream.h>
#include <Framework/Media/FrMediaPacket.h>
#include <Framework/Media/FrIMediaRender.h>
#include <Framework/Device/FrAudioDevice.h>
#include <Framework/Device/FrWindow.h>

static void i_media_render_imp(FrIMediaRenderInterface *iface);

SYS_DEFINE_WITH_CODE(FrAudioStream, fr_audio_stream, FR_TYPE_MEDIA_STREAM,
    SYS_IMPLEMENT_INTERFACE(FR_TYPE_I_MEDIA_RENDER, i_media_render_imp));

FrMediaStream *fr_audio_stream_new_out_by_device(FrAudioDevice *dev) {
  FrAudioStreamContext info = {0};

  fr_audio_device_get_info(dev,
      &info.channels,
      &info.sample_rate,
      &info.format);
  info.dev = dev;

  return fr_audio_stream_new_I(&info);
}

static void fr_audio_stream_render(FrIMediaRender *o,
    FrMediaFrame *frame, 
    SysPointer user_data) {
}

static void i_media_render_imp(FrIMediaRenderInterface *iface) {

  iface->render = fr_audio_stream_render;
}

/* object api */
static void fr_audio_stream_construct_i(FrMediaStream *o,
    FrAudioStreamContext *info) {
  FrAudioStream *self = FR_AUDIO_STREAM(o);

  fr_media_audio_stream_create(self, info);
  fr_window_audio_stream_create(self, info);
}

FrMediaStream* fr_audio_stream_new(void) {
  return sys_object_new(FR_TYPE_AUDIO_STREAM, NULL);
}

FrMediaStream *fr_audio_stream_new_I(FrAudioStreamContext *info) {
  sys_return_val_if_fail(info != NULL, NULL);
  sys_return_val_if_fail(info->channels != 0, NULL);
  sys_return_val_if_fail(info->sample_rate != 0, NULL);
  sys_return_val_if_fail(info->dev != NULL, NULL);

  FrMediaStream *o = fr_audio_stream_new();

  fr_audio_stream_construct_i(o, info);

  return o;
}

static void fr_audio_stream_dispose(SysObject* o) {
  FrAudioStream *self = FR_AUDIO_STREAM(o);

  fr_window_audio_stream_free(self);

  SYS_OBJECT_CLASS(fr_audio_stream_parent_class)->dispose(o);
}

static void fr_audio_stream_class_init(FrAudioStreamClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_audio_stream_dispose;
}

static void fr_audio_stream_init(FrAudioStream* self) {
}
