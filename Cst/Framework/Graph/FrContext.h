#ifndef __FR_CONTEXT_H__
#define __FR_CONTEXT_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_CONTEXT (fr_context_get_type())
#define FR_CONTEXT(o) ((FrContext* )sys_object_cast_check(o, FR_TYPE_CONTEXT))
#define FR_CONTEXT_CLASS(o) ((FrContextClass *)sys_class_cast_check(o, FR_TYPE_CONTEXT))
#define FR_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, FrContextClass)

struct _FrContextClass {
  SysObjectClass parent;
};

struct _FrContext {
  SysObject parent;

  /* <private> */
  FrSurface *surface;
  SysPointer ctx;
  PangoLayout *layout;
};

SYS_API SysType fr_context_get_type(void);
SYS_API FrContext *fr_context_new(void);

SYS_API FrContext *fr_context_new_I(FrSurface *surface);
SYS_API void fr_context_stroke_mp(FrContext* self, const FrBound *bound, const FrSInt4* m4, const FrSInt4* p4);
SYS_API void fr_context_rectangle_red(FrContext* cr, SysInt x, SysInt y);
SYS_API void fr_context_stoke_debug(FrContext* cr, SysInt i);
SYS_API void fr_context_fill_bound(FrContext* self, const FrBound* bound);

SYS_END_DECLS

#endif
