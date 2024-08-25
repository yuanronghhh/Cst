#include <Framework/Media/FrVideoFrame.h>
#include <Framework/Media/FrImageScale.h>

SYS_DEFINE_TYPE(FrVideoFrame, fr_video_frame, FR_TYPE_MEDIA_FRAME);

SysBool fr_video_frame_scale(FrVideoFrame *self, FrImageScale *scale) {
  sys_return_val_if_fail(self != NULL, false);

  FrMediaFrame *mframe = FR_MEDIA_FRAME(self);
  fr_video_frame_set_out_size(self, scale->out_width, scale->out_height);

#if 0
  const SysChar *filename = FR_PROJECT_DIR"/Assets/surface.png";
  fr_media_rgba_save_to_png(nframe, filename);
#endif

  return fr_image_scale_scale_media_frame(scale, mframe) == 0;
}

void fr_video_frame_init_frame_i(FrMediaFrame* o, FrMediaStream *stream) {
  sys_return_if_fail(o != NULL);

  FrVideoFrame *self = FR_VIDEO_FRAME(o);
  FR_MEDIA_FRAME_CLASS(fr_video_frame_parent_class)->init_frame(o, stream);

  fr_media_video_frame_init(self, stream);
  // sys_debug_N("%lld", o->timestamp);
}

void fr_video_frame_set_out_size(FrVideoFrame* self, SysInt width, SysInt height) {
  sys_return_if_fail(self);

  self->width = width;
  self->height = height;
}

void fr_video_frame_get_out_size(FrVideoFrame* self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self);

  *width = self->width;
  *height = self->height;
}

void fr_video_frame_get_frame_data(FrVideoFrame *self,
  SysUInt8 *frame_data[],
  SysInt linesize[]) {
  sys_return_if_fail(self != NULL);

  fr_media_frame_get_data(&self->parent, frame_data);
  fr_media_frame_get_linesize(&self->parent, linesize);
}

SysObject* fr_video_frame_dclone_i(SysObject* o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysObject* n;

  n = SYS_OBJECT_CLASS(fr_video_frame_parent_class)->dclone(o);

  FrVideoFrame* nself = FR_VIDEO_FRAME(n);
  FrVideoFrame* oself = FR_VIDEO_FRAME(o);

  nself->width = oself->width;
  nself->height = oself->height;
  nself->window = oself->window ? sys_object_ref(oself->window) : NULL;

  return n;
}

/* object api */
static void fr_video_frame_construct(FrVideoFrame *self) {
}

FrVideoFrame* fr_video_frame_new(void) {
  return sys_object_new(FR_TYPE_VIDEO_FRAME, NULL);
}

FrVideoFrame *fr_video_frame_new_I(void) {
  FrVideoFrame *o = fr_video_frame_new();

  fr_video_frame_construct(o);

  return o;
}

static void fr_video_frame_dispose(SysObject* o) {


}

static void fr_video_frame_class_init(FrVideoFrameClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrMediaFrameClass *mcls = FR_MEDIA_FRAME_CLASS(cls);

  ocls->dispose = fr_video_frame_dispose;
  ocls->dclone = fr_video_frame_dclone_i;

  mcls->init_frame = fr_video_frame_init_frame_i;
}

void fr_video_frame_init(FrVideoFrame* self) {
}
