#ifndef __FR_SURFACE_H__
#define __FR_SURFACE_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_SURFACE (fr_surface_get_type())
#define FR_SURFACE(o) ((FrSurface* )sys_object_cast_check(o, FR_TYPE_SURFACE))
#define FR_SURFACE_CLASS(o) ((FrSurfaceClass *)sys_class_cast_check(o, FR_TYPE_SURFACE)
#define FR_SURFACE_GET_CLASS(o) sys_instance_get_class(o, FrSurfaceClass)

struct _FrSurfaceClass {
  SysObjectClass parent;
};

struct _FrSurface {
  SysObject parent;

  /* <private> */
  FrDrawSurface *surface;
};

SYS_API SysType fr_surface_get_type(void);
SYS_API FrSurface *fr_surface_new(void);
SYS_API FrSurface *fr_surface_new_I(FrDrawSurface *dsurface);
SYS_API FrSurface *fr_surface_new_device(FrIDevice *device, SysInt width, SysInt height);
SYS_API FrContext *fr_surface_create_draw_cr(FrSurface *self);
SYS_API FrSurface *fr_surface_create_image_surface_from_surface(FrSurface *surface, width, height);

SYS_END_DECLS

#endif
