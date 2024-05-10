#include <Framework/Graph/FrSurface.h>
#include <Framework/Device/FrIDevice.h>
#include <Framework/Graph/FrDraw.h>
#include <Framework/Graph/FrContext.h>


SYS_DEFINE_TYPE(FrSurface, fr_surface, SYS_TYPE_OBJECT);

FrSurface *fr_surface_update_surface(FrSurface *self,
    FrSurface *device_surface,
    SysInt width,
    SysInt height) {

  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(device_surface != NULL, NULL);

  sys_assert(self->ctx != NULL);

  FrDrawSurface * new_surface =
    fr_i_draw_surface_create_similar_image(self->ctx, width, height);
  if(new_surface == NULL) { return NULL; }

  sys_clear_pointer(&self->ctx, fr_i_draw_surface_destroy);
  self->ctx = new_surface;

  return self;
}

void fr_surface_flush(FrSurface *self) {
  sys_return_if_fail(self != NULL);

  fr_i_draw_surface_flush(self->ctx);
}

FrSurface* fr_surface_create_device_surface_full(FrIDevice *device,
    SysInt width, 
    SysInt height) {
  FrDrawSurface* draw_surface = fr_i_draw_create_surface(device, width, height);

  return fr_surface_create_draw_surface(draw_surface);
}

FrSurface* fr_surface_create_device_surface(FrIDevice* device) {
  SysInt width = 0, height = 0;

  fr_i_device_get_size(device, &width, &height);

  return fr_surface_create_device_surface_full(device, width, height);
}


/* object api */
static void fr_surface_construct(FrSurface *self, FrDrawSurface *draw_surface) {
  self->ctx = draw_surface;
}

FrSurface* fr_surface_new(void) {
  return sys_object_new(FR_TYPE_SURFACE, NULL);
}

FrSurface *fr_surface_create_draw_surface(FrDrawSurface *draw_surface) {
  FrSurface *o = fr_surface_new();

  fr_surface_construct(o, draw_surface);

  return o;
}

FrSurface *fr_surface_create_surface(FrIDevice *device,
    SysInt width, 
    SysInt height) {
  FrSurface *o;

  FrIDrawInterface *iface  = fr_draw_get_iface();
  FrDrawSurface * draw_surface = iface->create_surface(device, width, height);

  o = fr_surface_create_draw_surface(draw_surface);
  fr_surface_construct(o, draw_surface);

  return o;
}

static void fr_surface_dispose(SysObject* o) {
  FrSurface *self = FR_SURFACE(o);

  sys_clear_pointer(&self->ctx, fr_i_draw_surface_destroy);

  SYS_OBJECT_CLASS(fr_surface_parent_class)->dispose(o);
}

static void fr_surface_class_init(FrSurfaceClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_surface_construct;
  ocls->dispose = fr_surface_dispose;
}

void fr_surface_init(FrSurface* self) {
}

