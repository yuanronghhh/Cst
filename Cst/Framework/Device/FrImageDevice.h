#ifndef __FR_IMAGE_DEVICE_H__
#define __FR_IMAGE_DEVICE_H__

#include <Framework/Device/FrDevice.h>

SYS_BEGIN_DECLS

#define FR_TYPE_IMAGE_DEVICE (fr_image_device_get_type())
#define FR_IMAGE_DEVICE(o) ((FrImageDevice* )sys_object_cast_check(o, FR_TYPE_IMAGE_DEVICE))
#define FR_IMAGE_DEVICE_CLASS(o) ((FrImageDeviceClass *)sys_class_cast_check(o, FR_TYPE_IMAGE_DEVICE))
#define FR_IMAGE_DEVICE_GET_CLASS(o) sys_instance_get_class(o, FrImageDeviceClass)

typedef struct _FrImageDevice FrImageDevice;
typedef struct _FrImageDeviceClass FrImageDeviceClass;

struct _FrImageDeviceClass {
  FrDeviceClass parent;
};

struct _FrImageDevice {
  FrDevice parent;

  /* <private> */
  FrImage image;
};

SYS_API SysType fr_image_device_get_type(void);
SYS_API FrDevice *fr_image_device_new(void);

SYS_API FrDevice *fr_image_device_new_I(void);

SYS_END_DECLS

#endif
