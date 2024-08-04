#ifndef __FR_MEDIA_RENDER_H__
#define __FR_MEDIA_RENDER_H__

#include <Framework//FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_MEDIA_RENDER (fr_media_render_get_type())
#define FR_MEDIA_RENDER(o) ((FrMediaRender* )sys_object_cast_check(o, FR_TYPE_MEDIA_RENDER))
#define FR_MEDIA_RENDER_CLASS(o) ((FrMediaRenderClass *)sys_class_cast_check(o, FR_TYPE_MEDIA_RENDER))
#define FR_MEDIA_RENDER_GET_CLASS(o) sys_instance_get_class(o, FrMediaRenderClass)

struct _FrMediaRenderClass {
  SysObjectClass parent;
};

struct _FrMediaRender {
  SysObject parent;

  /* <private> */
  FrDrawContext *video_render;
  FrAudioStream *audio_render;
};

struct _FrMediaRenderContext {
  FrDrawContext *video_render;
  FrAudioStream *audio_render;
};

SYS_API SysType fr_media_render_get_type(void);
SYS_API FrMediaRender *fr_media_render_new(void);

SYS_API FrMediaRender *fr_media_render_new_I(FrMediaRenderContext *info);

void fr_media_render_render_video (FrMediaRender *self,  FrVideoFrame *vframe, FrRegion *region);
void fr_media_render_render_audio (FrMediaRender *self,  FrAudioFrame *aframe);

SYS_END_DECLS

#endif
