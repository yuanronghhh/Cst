#include <Framework/Device/FrDevice.h>
#include <Framework/Graph/FrSurface.h>

SYS_DEFINE_TYPE(FrDevice, fr_device, SYS_TYPE_OBJECT);

FrSurface* fr_device_create_surface_by_type (FrDevice* self, 
    SysType surface_type,
    FrSurfaceContext *info) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(surface_type != 0, NULL);
  FrSurface *o;

  o = sys_object_new(surface_type, NULL);
  fr_surface_construct(o, info);

  return o;
}

FrSurface* fr_device_create_surface (FrDevice* self, 
    FrSurfaceContext *info) {
  sys_return_val_if_fail(self != NULL, NULL);
  FrSurface *o;

  o = fr_device_create_surface_by_type(self,
      self->surface_type, 
      info);

  return o;
}

/* object api */
static void fr_device_construct_i(FrDevice *self) {

}

FrDevice* fr_device_new(void) {
  return sys_object_new(FR_TYPE_DEVICE, NULL);
}

FrDevice *fr_device_new_I(void) {
  FrDevice *o = fr_device_new();

  fr_device_construct_i(o);

  return o;
}

static void fr_device_dispose(SysObject* o) {
  FrDevice *self = FR_DEVICE(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(fr_device_parent_class)->dispose(o);
}

static void fr_device_class_init(FrDeviceClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_device_dispose;
}

void fr_device_init(FrDevice* self) {
  self->surface_type = FR_TYPE_SURFACE;
}
