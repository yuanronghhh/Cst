#ifndef __FR_AV_RENDER_H__
#define __FR_AV_RENDER_H__

#include <Framework//FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AV_RENDER (fr_av_render_get_type())
#define FR_AV_RENDER(o) ((FrAvRender* )sys_object_cast_check(o, FR_TYPE_AV_RENDER))
#define FR_AV_RENDER_CLASS(o) ((FrAvRenderClass *)sys_class_cast_check(o, FR_TYPE_AV_RENDER))
#define FR_AV_RENDER_GET_CLASS(o) sys_instance_get_class(o, FrAvRenderClass)

struct _FrAvRenderClass {
  SysObjectClass parent;
};

struct _FrAvRender {
  SysObject parent;

  /* <private> */
  FrDrawContext *video_render;
  FrAudioStream *audio_render;
};

struct _FrAvRenderContext {
  FrDrawContext *video_render;
  FrAudioStream *audio_render;
};

SYS_API SysType fr_av_render_get_type(void);
SYS_API FrAvRender *fr_av_render_new(void);

SYS_API FrAvRender *fr_av_render_new_I(FrAvRenderContext *info);

void fr_av_render_render_video (FrAvRender *self,  FrVideoFrame *vframe, FrRegion *region);
void fr_av_render_render_audio (FrAvRender *self,  FrAudioFrame *aframe);

SYS_END_DECLS

#endif
