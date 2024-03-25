#include <Framework/Device/FrIDevice.h>

SYS_DEFINE_INTERFACE(FrIDevice, fr_i_device, SYS_TYPE_OBJECT);

/* object api */
void fr_i_device_default_init(FrIDeviceInterface* iface) {
}

void fr_i_device_get_size (FrIDevice *self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);

  FR_I_DEVICE_GET_IFACE(self)->get_size(self, width, height);
}
