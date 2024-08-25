#include <Framework/Device/FrImageDevice.h>

SYS_DEFINE_TYPE(FrImageDevice, fr_image_device, FR_TYPE_DEVICE);

/* object api */
static void fr_image_device_construct_i(FrDevice *self) {

}

FrDevice* fr_image_device_new(void) {
  return sys_object_new(FR_TYPE_IMAGE_DEVICE, NULL);
}

FrDevice *fr_image_device_new_I(void) {
  FrDevice *o = fr_image_device_new();

  fr_image_device_construct_i(o);

  return o;
}

static void fr_image_device_dispose(SysObject* o) {
  FrImageDevice *self = FR_IMAGE_DEVICE(o);

  UNUSED(self);


}

static void fr_image_device_class_init(FrImageDeviceClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_image_device_dispose;
}

void fr_image_device_init(FrImageDevice* self) {
}
