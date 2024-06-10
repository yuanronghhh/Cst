#include <Framework/Media/FrVideoFrame.h>
#include <Framework/Media/FrImageScale.h>

SYS_DEFINE_TYPE(FrVideoFrame, fr_video_frame, FR_TYPE_MEDIA_FRAME);

SysBool fr_video_frame_scale(FrVideoFrame *self, FrImageScale *scale) {
  sys_return_val_if_fail(self != NULL, false);

  AVFrame* rgba_frame = fr_media_new_agba_frame(self->width, self->height);
  if (rgba_frame == NULL) { return false; }

  if (fr_media_avframe_convert(scale, self->parent.ctx, rgba_frame) < 0) {
    av_frame_free(&rgba_frame);
    goto done;
  }
  self->parent.ctx = rgba_frame;

  fr_video_frame_set_out_size(self, self->width, self->height);

done:
  return self;
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
  SysUInt8 **frame_data[], 
  SysInt *linesize[]) {
  sys_return_if_fail(self != NULL);

  *frame_data = self->parent.ctx->data;
  *linesize = self->parent.ctx->linesize;
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

  SYS_OBJECT_CLASS(fr_video_frame_parent_class)->dispose(o);
}

static void fr_video_frame_class_init(FrVideoFrameClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_video_frame_dispose;
}

void fr_video_frame_init(FrVideoFrame* self) {
}
