#include <Framework/Graph/FrSurface.h>
#include <Framework/Graph/FrIDraw.h>
#include <Framework/Graph/FrContext.h>
#include <Framework/Device/FrDevice.h>
#include <Framework/Device/FrIDevice.h>

SYS_DEFINE_TYPE(FrSurface, fr_surface, SYS_TYPE_OBJECT);


FrSurface* fr_surface_create_device_surface(FrIDevice* idevice) {
  SysInt width = 0, height = 0;
  FrDevice *device = FR_DEVICE(idevice);

  fr_i_device_get_size(idevice, &width, &height);
  FrSurfaceContext info = { .width = width, .height = height };

  return fr_device_create_surface(device, &info);
}

void fr_surface_construct(FrSurface* self, FrSurfaceContext *info) {
  sys_return_if_fail(self != NULL);

  FrSurfaceClass* cls = FR_SURFACE_GET_CLASS(self);
  sys_return_if_fail(cls->construct);

  cls->construct(self, info);
}

/* object api */
static void fr_surface_construct_i(FrSurface *self,
    FrSurfaceContext *info) {

  fr_i_draw_surface_create(self, info);
}

FrSurface* fr_surface_new(void) {
  return sys_object_new(FR_TYPE_SURFACE, NULL);
}

FrSurface* fr_surface_new_I(FrSurfaceContext* info) {
  FrSurface* o = fr_surface_new();
  fr_surface_construct_i(o, info);
  return o;
}

static void fr_surface_dispose(SysObject* o) {
  FrSurface *self = FR_SURFACE(o);

  fr_i_draw_surface_destroy(self);


}

static void fr_surface_class_init(FrSurfaceClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_surface_construct_i;
  ocls->dispose = fr_surface_dispose;
}

void fr_surface_init(FrSurface* self) {
}

