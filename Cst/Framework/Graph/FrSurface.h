#ifndef __FR_SURFACE_H__
#define __FR_SURFACE_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_SURFACE (fr_surface_get_type())
#define FR_SURFACE(o) ((FrSurface* )sys_object_cast_check(o, FR_TYPE_SURFACE))
#define FR_SURFACE_CLASS(o) ((FrSurfaceClass *)sys_class_cast_check(o, FR_TYPE_SURFACE))
#define FR_SURFACE_GET_CLASS(o) sys_instance_get_class(o, FrSurfaceClass)

struct _FrSurfaceClass {
  SysObjectClass parent;

  void (*construct) (FrSurface *self, FrSurfaceContext *info);
};

struct _FrSurface {
  SysObject parent;

  /* <private> */
  SysPointer ctx;
};

struct _FrSurfaceContext {
  SysInt width;
  SysInt height;
};

SYS_API SysType fr_surface_get_type(void);
void fr_surface_construct(FrSurface* self, FrSurfaceContext *info);
SYS_API FrSurface *fr_surface_new(void);

SYS_END_DECLS

#endif
