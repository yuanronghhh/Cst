#ifndef __FR_I_MEDIA_RENDER__
#define __FR_I_MEDIA_RENDER__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_I_MEDIA_RENDER (fr_i_media_render_get_type())
#define FR_I_MEDIA_RENDER(o) ((FrIMediaRender* )sys_object_cast_check(o, FR_TYPE_I_MEDIA_RENDER))
#define FR_I_MEDIA_RENDER_GET_IFACE(o) ((FrIMediaRenderInterface *)SYS_TYPE_GET_INTERFACE(o, FR_TYPE_I_MEDIA_RENDER))

struct _FrIMediaRenderInterface  {
  SysTypeInterface unowned;

  void (*render) (FrIMediaRender *self, FrMediaFrame *frame, SysPointer user_data);
};

SysType fr_i_media_render_get_type(void);

void fr_i_media_render_render (FrIMediaRender *self, FrMediaFrame *frame, SysPointer user_data);


SYS_END_DECLS

#endif
