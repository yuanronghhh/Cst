#ifndef __FR_I_DEVICE__
#define __FR_I_DEVICE__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_I_DEVICE (fr_i_device_get_type())
#define FR_I_DEVICE(o) ((FrIDevice* )sys_object_cast_check(o, FR_TYPE_I_DEVICE))
#define FR_I_DEVICE_GET_IFACE(o) ((FrIDeviceInterface *)SYS_TYPE_GET_INTERFACE(o, FR_TYPE_I_DEVICE))


struct _FrIDeviceInterface {
  SysTypeInterface parent;

  /* <private> */
  void (*get_size) (FrIDevice* self, SysInt* width, SysInt* height);

  /* window */
};

SysType fr_i_device_get_type(void);
void fr_i_device_get_size (FrIDevice *self, SysInt *width, SysInt *height);

SYS_END_DECLS

#endif
