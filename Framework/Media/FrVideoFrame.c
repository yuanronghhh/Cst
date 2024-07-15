#include <Framework/Media/FrVideoFrame.h>
#include <Framework/Media/FrImageScale.h>

SYS_DEFINE_TYPE(FrVideoFrame, fr_video_frame, FR_TYPE_MEDIA_FRAME);

SysBool fr_video_frame_scale(FrVideoFrame *self, FrImageScale *scale) {
  sys_return_val_if_fail(self != NULL, false);

  AVFrame* nframe = fr_media_new_rgba_frame(self->width, self->height, scale->out_pix_fmt);
  if (nframe == NULL) { return false; }

  if (fr_image_scale_convert_avframe(scale, self->parent.ctx, nframe) < 0) {
    sys_warning_N("convert avframe failed: %p", self);
    av_frame_free(&nframe);
    goto done;
  }
  av_frame_free(&self->parent.ctx);
  self->parent.ctx = nframe;

#if 0
  const SysChar *filename = FR_PROJECT_DIR"/Assets/surface.png";
  fr_media_rgba_save_to_png(nframe, filename);
#endif

  fr_video_frame_set_out_size(self, self->width, self->height);

done:
  return self;
}

void fr_video_frame_init_frame(FrVideoFrame* self) {
  sys_return_if_fail(self != NULL);

  AVFrame *frame = self->parent.ctx;
  self->width = frame->width;
  self->height = frame->height;
  self->format = frame->format;
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

SysObject* fr_video_frame_dclone_i(SysObject* o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysObject* n;

  n = SYS_OBJECT_CLASS(fr_video_frame_parent_class)->dclone(o);

  FrVideoFrame* nself = FR_VIDEO_FRAME(n);
  FrVideoFrame* oself = FR_VIDEO_FRAME(o);

  nself->width = oself->width;
  nself->height = oself->height;
  nself->format = oself->format;
  nself->window = oself->window ? sys_object_ref(oself->window) : NULL;
  nself->delay = oself->delay;

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

  SYS_OBJECT_CLASS(fr_video_frame_parent_class)->dispose(o);
}

static void fr_video_frame_class_init(FrVideoFrameClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_video_frame_dispose;
  ocls->dclone = fr_video_frame_dclone_i;
}

void fr_video_frame_init(FrVideoFrame* self) {
}
