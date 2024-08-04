#include <Framework/Media/FrMediaRender.h>
#include <Framework/Graph/FrDrawContext.h>

SYS_DEFINE_TYPE(FrMediaRender, fr_media_render, SYS_TYPE_OBJECT);


void fr_media_render_render_video (FrMediaRender *self,  FrVideoFrame *vframe, FrRegion *region) {
  sys_return_if_fail(self->video_render != NULL);

  fr_draw_context_render_video(self->video_render, vframe, region);
}

void fr_media_render_render_audio (FrMediaRender *self, FrAudioFrame *aframe) {
  // sys_return_if_fail(self->audio_render != NULL);
}

/* object api */
static void fr_media_render_construct_i(FrMediaRender *self, FrMediaRenderContext *info) {
  self->video_render = info->video_render ? sys_object_ref(info->video_render) : NULL;
  self->audio_render = info->audio_render ? sys_object_ref(info->audio_render) : NULL;
}

FrMediaRender* fr_media_render_new(void) {
  return sys_object_new(FR_TYPE_MEDIA_RENDER, NULL);
}

FrMediaRender *fr_media_render_new_I(FrMediaRenderContext *info) {
  FrMediaRender *o = fr_media_render_new();

  fr_media_render_construct_i(o, info);

  return o;
}

static void fr_media_render_dispose(SysObject* o) {
  FrMediaRender *self = FR_MEDIA_RENDER(o);

  sys_object_unref(self->video_render);
  sys_object_unref(self->audio_render);

  SYS_OBJECT_CLASS(fr_media_render_parent_class)->dispose(o);
}

static void fr_media_render_class_init(FrMediaRenderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_media_render_dispose;
}

static void fr_media_render_init(FrMediaRender* self) {
}
