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

  void (*construct) (FrSurface *self, FrDrawSurface* draw_surface);
};

struct _FrSurface {
  SysObject parent;

  /* <private> */
  FrDrawSurface *draw_surface;
};

SYS_API SysType fr_surface_get_type(void);
SYS_API FrSurface *fr_surface_new(void);
SYS_API FrSurface *fr_surface_create_draw_surface(FrDrawSurface *dsurface);
SYS_API FrSurface* fr_surface_create_device_surface(FrIDevice* device);
SYS_API FrSurface* fr_surface_create_device_surface_full(FrIDevice* device, SysInt width, SysInt height);
SYS_API FrSurface *fr_surface_create_image_surface_from_surface(FrSurface *surface, SysInt width, SysInt height);
SYS_API FrSurface *fr_surface_image_surface_create(SysInt width, SysInt height);
SYS_API void fr_surface_set_source_surface(FrSurface *surface, FrSurface *dst_surface);

SYS_API void fr_surface_set_draw_surface(FrSurface *self, FrDrawSurface * draw_surface);
SYS_API FrDrawSurface * fr_surface_get_draw_surface(FrSurface *self);
SYS_API void fr_surface_flush(FrSurface *self);
SYS_API FrSurface *fr_surface_update_surface(FrSurface *self, FrSurface *idevice_surface, SysInt width, SysInt height);

SYS_END_DECLS

#endif
