#include <Framework/Media/FrAvRender.h>
#include <Framework/Media/FrAudioStream.h>
#include <Framework/Graph/FrDrawContext.h>

SYS_DEFINE_TYPE(FrAvRender, fr_av_render, SYS_TYPE_OBJECT);


void fr_av_render_render_video (FrAvRender *self,  FrVideoFrame *vframe, FrRegion *region) {
  sys_return_if_fail(self->video_render != NULL);

  fr_draw_context_render_video(self->video_render, vframe, region);
}

void fr_av_render_render_audio (FrAvRender *self, FrAudioFrame *aframe) {
  sys_return_if_fail(self->audio_render != NULL);

  FrAudioStream *as = self->audio_render;

  UNUSED(as);
}

/* object api */
static void fr_av_render_construct_i(FrAvRender *self, FrAvRenderContext *info) {
  self->video_render = info->video_render ? sys_object_ref(info->video_render) : NULL;
  self->audio_render = info->audio_render ? sys_object_ref(info->audio_render) : NULL;
}

FrAvRender* fr_av_render_new(void) {
  return sys_object_new(FR_TYPE_AV_RENDER, NULL);
}

FrAvRender *fr_av_render_new_I(FrAvRenderContext *info) {
  FrAvRender *o = fr_av_render_new();
  sys_return_val_if_fail(info != NULL, NULL);

  if(info->audio_render == NULL
      && info->video_render == NULL) {
    return NULL;
  }

  fr_av_render_construct_i(o, info);

  return o;
}

static void fr_av_render_dispose(SysObject* o) {
  FrAvRender *self = FR_AV_RENDER(o);

  if(self->video_render) {

    sys_clear_pointer(&self->video_render, _sys_object_unref);
  }

  if(self->audio_render) {

    sys_clear_pointer(&self->audio_render, _sys_object_unref);
  }


}

static void fr_av_render_class_init(FrAvRenderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_av_render_dispose;
}

static void fr_av_render_init(FrAvRender* self) {
}
