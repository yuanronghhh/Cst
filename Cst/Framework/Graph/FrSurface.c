#include <Framework/Graph/FrSurface.h>
#include <Framework/Device/FrIDevice.h>
#include <Framework/Graph/FrIDraw.h>
#include <Framework/Graph/FrDraw.h>
#include <Framework/Graph/FrContext.h>


SYS_DEFINE_TYPE(FrSurface, fr_surface, SYS_TYPE_OBJECT);


FrContext *fr_surface_create_draw_cr(FrSurface *self) {
  sys_return_val_if_fail(self != NULL, NULL);
  FrContext *ctx = fr_context_new_I(self);

  return ctx;
}

FrSurface *fr_surface_create_image_surface_from_surface(FrSurface *surface, SysInt width, SysInt height) {
  FrDrawSurface * dsurface = fr_i_draw_create_image_surface_from_surface(surface->surface, width, height);

  return fr_surface_new_I(dsurface);
}

/* object api */
static void fr_surface_construct(FrSurface *self, FrDrawSurface *dsurface) {
  self->surface = dsurface;
}

FrSurface* fr_surface_new(void) {
  return sys_object_new(FR_TYPE_SURFACE, NULL);
}

FrSurface *fr_surface_new_I(FrDrawSurface *dsurface) {
  FrSurface *o = fr_surface_new();

  fr_surface_construct(o, dsurface);

  return o;
}

FrSurface *fr_surface_new_device(FrIDevice *device, SysInt width, SysInt height) {
  FrSurface *o;

  FrIDrawInterface *iface  = fr_draw_get_iface();
  FrDrawSurface * dsurface = iface->create_surface(device, width, height);

  o = fr_surface_new_I(dsurface);
  fr_surface_construct(o, dsurface);

  return o;
}

static void fr_surface_dispose(SysObject* o) {
  FrSurface *self = FR_SURFACE(o);

  SYS_OBJECT_CLASS(fr_surface_parent_class)->dispose(o);
}

static void fr_surface_class_init(FrSurfaceClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_surface_dispose;
}

void fr_surface_init(FrSurface* self) {
}

