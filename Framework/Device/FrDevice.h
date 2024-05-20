#ifndef __FR_DEVICE_H__
#define __FR_DEVICE_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_DEVICE (fr_device_get_type())
#define FR_DEVICE(o) ((FrDevice* )sys_object_cast_check(o, FR_TYPE_DEVICE))
#define FR_DEVICE_CLASS(o) ((FrDeviceClass *)sys_class_cast_check(o, FR_TYPE_DEVICE))
#define FR_DEVICE_GET_CLASS(o) sys_instance_get_class(o, FrDeviceClass)

struct _FrDeviceClass {
  SysObjectClass parent;

  FrSurface* (*create_surface) (FrDevice* idevice, SysInt width, SysInt height);
  void (*get_size) (FrDevice* idevice, SysInt *width, SysInt *height);
};

struct _FrDevice {
  SysObject parent;

  /* <private> */
  SysType surface_type;
};

SYS_API SysType fr_device_get_type(void);
SYS_API FrDevice *fr_device_new(void);

SYS_API FrDevice *fr_device_new_I(void);

FrSurface* fr_device_create_surface (FrDevice* self, 
    FrSurfaceContext *info);

void fr_device_get_size (FrDevice* idevice, SysInt *width, SysInt *height);

FrSurface* fr_device_create_surface_by_type (FrDevice* self, 
    SysType surface_type,
    FrSurfaceContext *info);

SYS_END_DECLS

#endif
