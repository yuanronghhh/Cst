#include <Framework/Graph/FrDraw.h>
#include <Framework/Graph/FrIDraw.h>
#include <Framework/Graph/FrContext.h>
#include <Framework/Graph/FrCairoDraw.h>
#include <Framework/Graph/FrSurface.h>
#include <Framework/Device/FrDisplay.h>
#include <Framework/Device/FrIDevice.h>

static FrDraw *g_draw = NULL;

SYS_DEFINE_TYPE(FrDraw, fr_draw, SYS_TYPE_OBJECT);

SysBool fr_draw_frame_need_draw(FrDraw *self) {
  sys_return_val_if_fail(self != NULL, false);

  return !self->is_painting;
}

void fr_draw_set_is_painting(FrDraw *self, SysBool is_painting) {
  sys_return_if_fail(self != NULL);

  self->is_painting = is_painting;
}

SysBool fr_draw_get_is_painting(FrDraw *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->is_painting;
}

static void prepare_paint_surface(FrIDevice *idevice, FrSurface *surface, FrRegion *region) {
  FrContext *cr = fr_context_new_I(surface);
  SysInt width = 0, height = 0;

  fr_i_device_get_size(idevice, &width, &height);
  fr_context_fill_background(cr, width, height);

  sys_object_unref(cr);
}

void fr_draw_frame_begin(FrDraw *self, FrRegion *region) {
  sys_return_if_fail(self != NULL);
  SysInt width = 0, height = 0;
  FrSurface *idevice_surface;
  FrSurface **p;
  SysUInt i = 0;

  fr_i_device_get_size(self->idevice, &width, &height);
  idevice_surface =  fr_surface_create_device_surface_full(self->idevice, width, height);

  p = (FrSurface **)(self->surfaces->pdata);
  while(i < self->surfaces->len) {
    sys_clear_pointer(p, _sys_object_unref);

    *p = fr_surface_create_image_surface_from_surface(idevice_surface, width, height);
    prepare_paint_surface(self->idevice, *p, region);
    p++;
  }

  self->idevice_surface = idevice_surface;
  self->is_painting = false;
}

void fr_draw_frame_end(FrDraw *self, FrRegion *region) {
  sys_return_if_fail(self != NULL);

  int n_boxes, i;
  cairo_rectangle_int_t box;
  FrContext* cr;

  cr = fr_context_new_I(self->idevice_surface);

  n_boxes = fr_region_num_rectangles(region);
  for (i = 0; i < n_boxes; i++) {
    fr_region_get_rectangle(region, i, &box);
    fr_context_rectangle(cr, box.x, box.y, box.width, box.height);
  }

  fr_context_rectangle(cr, box.x, box.y, box.width, box.height);
  fr_context_clip(cr);
  fr_context_paint(cr);
  sys_object_unref(cr);

  sys_clear_pointer(&cr, fr_context_destroy);
  fr_surface_flush(self->idevice_surface);

  sys_clear_pointer(&self->idevice_surface, _sys_object_unref);

  self->is_painting = true;
}

void fr_draw_get_buffer_size (FrDraw *self, SysInt *width, SysInt *height) {

  fr_i_device_get_size(self->idevice, width, height);
}

void fr_draw_set_surfaces(FrDraw *self, SysHArray * surfaces) {
  sys_return_if_fail(self != NULL);

  self->surfaces = surfaces;
  sys_object_ref(surfaces);
}

SysHArray * fr_draw_get_surfaces(FrDraw *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->surfaces;
}

FrSurface *fr_draw_get_surface_by_idx(FrDraw *self, SysInt idx) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->surfaces->pdata[idx];
}

void fr_draw_setup(const SysChar *name, FrIDevice *idevice) {
  sys_assert(g_draw == NULL);

  if(sys_str_equal(name, "cairo")) {
    g_draw = fr_cairo_draw_new_I(idevice);
  }
}

void fr_draw_teardown(void) {
  sys_assert(g_draw != NULL);
  sys_clear_pointer(&g_draw, _sys_object_unref);
}

FrIDrawInterface* fr_draw_get_iface(void) {
  FrIDrawInterface *iface = FR_I_DRAW_GET_IFACE(g_draw);

  return iface;
}

/* object api */
static void fr_draw_construct(FrDraw *self, FrIDevice *idevice) {
  self->idevice = idevice;
  self->idevice_surface = NULL;
  sys_object_ref(idevice);
}

FrDraw* fr_draw_new(void) {
  return sys_object_new(FR_TYPE_DRAW, NULL);
}

FrDraw *fr_draw_new_I(FrIDevice *idevice) {
  FrDraw *o = fr_draw_new();

  fr_draw_construct(o, idevice);

  return o;
}

static void fr_draw_dispose(SysObject* o) {
  FrDraw* self = FR_DRAW(o);

  if (self->idevice) {

    sys_clear_pointer(&self->idevice, _sys_object_unref);
  }

  SYS_OBJECT_CLASS(fr_draw_parent_class)->dispose(o);
}

static void fr_draw_class_init(FrDrawClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_draw_dispose;
}

void fr_draw_init(FrDraw *self) {
}
