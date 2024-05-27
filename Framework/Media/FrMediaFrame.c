#include <Framework/Media/FrMediaFrame.h>

SYS_DEFINE_TYPE(FrMediaFrame, fr_media_frame, FR_TYPE_PACKET);

SysBool fr_media_frame_destroy_i(SysObject *o) {
  FrMediaFrame* self = FR_MEDIA_FRAME(o);
  if(self->ctx == NULL) { return false; }

  av_frame_free(&self->ctx);

  return SYS_OBJECT_CLASS(fr_media_frame_parent_class)->destroy(o);
}

void fr_media_frame_set_ctx(FrMediaFrame *self, AVFrame * ctx) {
  sys_return_if_fail(self != NULL);

  self->ctx = ctx;
}

AVFrame * fr_media_frame_get_ctx(FrMediaFrame *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->ctx;
}

SysObject* fr_media_frame_dclone_i(SysObject* o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysObject* n = SYS_OBJECT_CLASS(fr_media_frame_parent_class)->dclone(o);

  FrMediaFrame* nself = FR_MEDIA_FRAME(n);
  FrMediaFrame* oself = FR_MEDIA_FRAME(o);

  av_frame_ref(nself->ctx, oself->ctx);

  return n;
}

/* object api */
FrMediaFrame* fr_media_frame_new(void) {
  return sys_object_new(FR_TYPE_MEDIA_FRAME, NULL);
}

static void fr_media_frame_dispose(SysObject* o) {
  FrMediaFrame* self = FR_MEDIA_FRAME(o);

  av_frame_free(&self->ctx);

  SYS_OBJECT_CLASS(fr_media_frame_parent_class)->dispose(o);
}

static void fr_media_frame_class_init(FrMediaFrameClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->destroy = fr_media_frame_destroy_i;
  ocls->dispose = fr_media_frame_dispose;
  ocls->dclone = fr_media_frame_dclone_i;
}

void fr_media_frame_init(FrMediaFrame* self) {
  self->ctx = av_frame_alloc();
}
