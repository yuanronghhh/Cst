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
void fr_context_fill_bound(FrContext* self, const FrRect* bound);
void fr_context_stroke_mp(FrContext* self, const FrRect* bound, const FrSInt4* m4, const FrSInt4* p4);
void fr_context_set_color(FrContext* cr, FrColor *color);
void fr_context_set_source_surface (FrContext* self, FrSurface* surface,SysDouble x,SysDouble y);
void fr_context_rectangle (FrContext* self,SysDouble x,SysDouble y,SysDouble width,SysDouble height);
void fr_context_clip (FrContext* self);
void fr_context_paint (FrContext* self);
void fr_context_destroy (FrContext* self);
void fr_context_move_to (FrContext* self,SysDouble x,SysDouble y);
void fr_context_save(FrContext* self);
void fr_context_restore(FrContext* self);

SYS_END_DECLS

#endif
