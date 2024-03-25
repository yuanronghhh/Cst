#ifndef __FR_CONTEXT_H__
#define __FR_CONTEXT_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_CONTEXT (fr_context_get_type())
#define FR_CONTEXT(o) ((FrContext* )sys_object_cast_check(o, FR_TYPE_CONTEXT))
#define FR_CONTEXT_CLASS(o) ((FrContextClass *)sys_class_cast_check(o, FR_TYPE_CONTEXT)
#define FR_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, FrContextClass)

struct _FrContextClass {
  SysObjectClass parent;
};

struct _FrContext {
  SysObject parent;

  /* <private> */
  FrDrawContext *cr;
};

SYS_API SysType fr_context_get_type(void);
SYS_API FrContext *fr_context_new(void);

SYS_API FrContext *fr_context_new_I(FrSurface *surface);
void fr_context_fill_background (FrContext *cr, SysInt width, SysInt height);

#define fr_context_restore fr_draw_context_restore
#define fr_context_save fr_draw_context_save
#define fr_context_set_source_surface cairo_set_source_surface
#define fr_context_destroy cairo_destroy
#define fr_context_destroy_surface cairo_surface_destroy
#define fr_context_update_layout pango_cairo_update_layout
#define fr_context_set_rgba cairo_set_source_rgba
#define fr_context_create cairo_create

SYS_END_DECLS

#endif
