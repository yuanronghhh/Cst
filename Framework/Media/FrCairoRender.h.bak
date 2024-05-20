#ifndef __FR_CAIRO_RENDER_H__
#define __FR_CAIRO_RENDER_H__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_CAIRO_RENDER (fr_cairo_render_get_type())
#define FR_CAIRO_RENDER(o) ((FrCairoRender* )sys_object_cast_check(o, FR_TYPE_CAIRO_RENDER))
#define FR_CAIRO_RENDER_CLASS(o) ((FrCairoRenderClass *)sys_class_cast_check(o, FR_TYPE_CAIRO_RENDER))
#define FR_CAIRO_RENDER_GET_CLASS(o) sys_instance_get_class(o, FrCairoRenderClass)

struct _FrCairoRenderClass {
  SysObjectClass parent;
};

struct _FrCairoRender {
  SysObject parent;

  /* <private> */
  FrContext *cr;
};

SYS_API SysType fr_cairo_render_get_type(void);
SYS_API FrCairoRender *fr_cairo_render_new(void);

FrCairoRender *fr_cairo_render_new_I(GLFWwindow *gwindow);

SYS_END_DECLS

#endif
