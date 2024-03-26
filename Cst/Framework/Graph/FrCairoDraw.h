#ifndef __FR_CAIRO_DRAW_H__
#define __FR_CAIRO_DRAW_H__

#include <Framework/Graph/FrDraw.h>

SYS_BEGIN_DECLS

#define FR_TYPE_CAIRO_DRAW (fr_cairo_draw_get_type())
#define FR_CAIRO_DRAW(o) ((FrCairoDraw* )sys_object_cast_check(o, FR_TYPE_CAIRO_DRAW))
#define FR_CAIRO_DRAW_CLASS(o) ((FrCairoDrawClass *)sys_class_cast_check(o, FR_TYPE_CAIRO_DRAW)
#define FR_CAIRO_DRAW_GET_CLASS(o) sys_instance_get_class(o, FrCairoDrawClass)

struct _FrCairoDrawClass {
  SysObjectClass parent;
};

struct _FrCairoDraw {
  SysObject parent;

  /* <private> */
};

SYS_API SysType fr_cairo_draw_get_type(void);
SYS_API FrDraw *fr_cairo_draw_new(void);
SYS_API FrDraw *fr_cairo_draw_new_I(FrIDevice *device);

SYS_END_DECLS

#endif
