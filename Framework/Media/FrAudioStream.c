#include <Framework/Media/FrAudioStream.h>
#include <Framework/Media/FrMediaPacket.h>
#include <Framework/Device/FrWindow.h>

SYS_DEFINE_TYPE(FrAudioStream, fr_audio_stream, FR_TYPE_MEDIA_STREAM);

void fr_audio_stream_resume(FrAudioStream *stream) {

  fr_window_audio_resume(stream->dev);
}

/* object api */
static void fr_audio_stream_construct_i(FrMediaStream *o,
    FrAudioStreamContext *info) {
  FrAudioStream *self = FR_AUDIO_STREAM(o);

  self->dev = sys_object_ref(info->dev);

  fr_window_audio_stream_create(self, info);
}

FrMediaStream* fr_audio_stream_new(void) {
  return sys_object_new(FR_TYPE_AUDIO_STREAM, NULL);
}

FrMediaStream *fr_audio_stream_new_I(FrAudioStreamContext *info) {
  sys_return_val_if_fail(info != NULL, NULL);
  sys_return_val_if_fail(info->dev != NULL, NULL);
  sys_return_val_if_fail(info->channels != 0, NULL);
  sys_return_val_if_fail(info->sample_rate != 0, NULL);

  FrMediaStream *o = fr_audio_stream_new();

  fr_audio_stream_construct_i(o, info);

  return o;
}

static void fr_audio_stream_dispose(SysObject* o) {
  FrAudioStream *self = FR_AUDIO_STREAM(o);

  sys_clear_pointer(self->dev, _sys_object_unref);
  sys_async_queue_clear_full(&self->queue);

  SYS_OBJECT_CLASS(fr_audio_stream_parent_class)->dispose(o);
}

static void fr_audio_stream_class_init(FrAudioStreamClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_audio_stream_dispose;
}

static void fr_audio_stream_init(FrAudioStream* self) {

  sys_async_queue_init_full(&self->queue, (SysDestroyFunc)_sys_object_unref);
}
