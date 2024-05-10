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
  FrDrawBrush *ctx;
  FrSurface *surface;
};

SYS_API SysType fr_context_get_type(void);
SYS_API FrContext *fr_context_new(void);

SYS_API FrContext *fr_context_new_I(FrSurface *surface);

SYS_END_DECLS

#endif
