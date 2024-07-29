#include <Framework/Media/FrAudioStream.h>

SYS_DEFINE_TYPE(FrAudioStream, fr_audio_stream, FR_TYPE_MEDIA_STREAM);

/* object api */
static void fr_audio_stream_construct_i(FrMediaStream *self, FrAudioStreamContext *info) {
}

FrMediaStream* fr_audio_stream_new(void) {
  return sys_object_new(FR_TYPE_AUDIO_STREAM, NULL);
}

FrMediaStream *fr_audio_stream_new_I(FrAudioStreamContext *info) {
  FrMediaStream *o = fr_audio_stream_new();

  fr_audio_stream_construct_i(o, info);

  return o;
}

static void fr_audio_stream_dispose(SysObject* o) {
  FrAudioStream *self = FR_AUDIO_STREAM(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(fr_audio_stream_parent_class)->dispose(o);
}

static void fr_audio_stream_class_init(FrAudioStreamClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_audio_stream_dispose;
}

static void fr_audio_stream_init(FrAudioStream* self) {
}
