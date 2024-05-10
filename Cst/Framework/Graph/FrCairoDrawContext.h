#ifndef __FR_CAIRO_DRAW_CONTEXT_H__
#define __FR_CAIRO_DRAW_CONTEXT_H__

#include <Framework/Graph/FrDrawContext.h>

SYS_BEGIN_DECLS

#define FR_TYPE_CAIRO_DRAW_CONTEXT (fr_cairo_draw_context_get_type())
#define FR_CAIRO_DRAW_CONTEXT(o) ((FrCairoDrawContext* )sys_object_cast_check(o, FR_TYPE_CAIRO_DRAW_CONTEXT))
#define FR_CAIRO_DRAW_CONTEXT_CLASS(o) ((FrCairoDrawContextClass *)sys_class_cast_check(o, FR_TYPE_CAIRO_DRAW_CONTEXT))
#define FR_CAIRO_DRAW_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, FrCairoDrawContextClass)

struct _FrCairoDrawContextClass {
  FrDrawContextClass parent;
};

struct _FrCairoDrawContext {
  FrDrawContext parent;

  /* <private> */
};

SYS_API SysType fr_cairo_draw_context_get_type(void);
SYS_API FrDrawContext *fr_cairo_draw_context_new(void);
SYS_API FrDrawContext *fr_cairo_draw_context_new_I(void);

SYS_END_DECLS

#endif
