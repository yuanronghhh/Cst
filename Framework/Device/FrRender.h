#ifndef __FR_RENDER_H__
#define __FR_RENDER_H__

#include <Framework//FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_RENDER (fr_render_get_type())
#define FR_RENDER(o) ((FrRender* )sys_object_cast_check(o, FR_TYPE_RENDER))
#define FR_RENDER_CLASS(o) ((FrRenderClass *)sys_class_cast_check(o, FR_TYPE_RENDER))
#define FR_RENDER_GET_CLASS(o) sys_instance_get_class(o, FrRenderClass)

struct _FrRenderClass {
  SysObjectClass parent;
};

struct _FrRender {
  SysObject parent;

  /* <private> */
  SysPointer ctx;
};

struct _FrRenderContext {
  SysPointer ctx;
};

SYS_API SysType fr_render_get_type(void);
SYS_API FrRender *fr_render_new(void);

SYS_API FrRender *fr_render_new_I(FrRenderContext *info);

SYS_END_DECLS

#endif
