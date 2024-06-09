#include <Framework/Media/FrAudioFrame.h>

SYS_DEFINE_TYPE(FrAudioFrame, fr_audio_frame, FR_TYPE_MEDIA_FRAME);

SysObject* fr_audio_frame_dclone_i(SysObject* o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysObject* n = SYS_OBJECT_CLASS(fr_audio_frame_parent_class)->dclone(o);

  /*
  FrAudioFrame* nself = FR_AUDIO_FRAME(n);
  FrAudioFrame* oself = FR_AUDIO_FRAME(o);
  */

  return n;
}


/* object api */
static void fr_audio_frame_construct(FrAudioFrame *self) {

}

FrAudioFrame* fr_audio_frame_new(void) {
  return sys_object_new(FR_TYPE_AUDIO_FRAME, NULL);
}

FrAudioFrame *fr_audio_frame_new_I(void) {
  FrAudioFrame *o = fr_audio_frame_new();

  fr_audio_frame_construct(o);

  return o;
}

static void fr_audio_frame_dispose(SysObject* o) {
  FrAudioFrame *self = FR_AUDIO_FRAME(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(fr_audio_frame_parent_class)->dispose(o);
}

static void fr_audio_frame_class_init(FrAudioFrameClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_audio_frame_dispose;
  ocls->dclone = fr_audio_frame_dclone_i;
}

void fr_audio_frame_init(FrAudioFrame* self) {
}
