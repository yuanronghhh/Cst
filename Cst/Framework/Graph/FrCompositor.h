#ifndef __FR_COMPOSITOR_H__
#define __FR_COMPOSITOR_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_COMPOSITOR (fr_compositor_get_type())
#define FR_COMPOSITOR(o) ((FrCompositor* )sys_object_cast_check(o, FR_TYPE_COMPOSITOR))
#define FR_COMPOSITOR_CLASS(o) ((FrCompositorClass *)sys_class_cast_check(o, FR_TYPE_COMPOSITOR))
#define FR_COMPOSITOR_GET_CLASS(o) sys_instance_get_class(o, FrCompositorClass)

struct _FrCompositorClass {
  SysObjectClass parent;
};

struct _FrCompositor {
  SysObject parent;

  /* <private> */
};

SYS_API SysType fr_compositor_get_type(void);
SYS_API FrCompositor *fr_compositor_new(void);

SYS_API FrCompositor *fr_compositor_new_I(void);
SYS_API void fr_compositor_surface_overlay(
  FrSurface* pn,
  FrContext* pb_cr);

SYS_END_DECLS

#endif
