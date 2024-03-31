#include <Framework/Graph/FrDrawContext.h>
#include <Framework/Graph/FrContext.h>
#include <Framework/Graph/FrIDraw.h>
#include <Framework/Graph/FrSurface.h>
#include <Framework/Device/FrIDevice.h>


SYS_DEFINE_TYPE(FrDrawContext, fr_draw_context, SYS_TYPE_OBJECT);


SysBool fr_draw_context_frame_need_draw(FrDrawContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return !self->is_painting;
}

void fr_draw_context_set_is_painting(FrDrawContext* self, SysBool is_painting) {
  sys_return_if_fail(self != NULL);

  self->is_painting = is_painting;
}

SysBool fr_draw_context_get_is_painting(FrDrawContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->is_painting;
}

static void prepare_paint_surface(FrIDevice* idevice, FrSurface* surface, FrRegion* region) {
  FrContext* cr = fr_context_new_I(surface);
  SysInt width = 0, height = 0;

  fr_i_device_get_size(idevice, &width, &height);
  fr_context_fill_background(cr, width, height);

  sys_object_unref(cr);
}

void fr_draw_context_get_buffer_size(FrDrawContext* self, SysInt* width, SysInt* height) {

  fr_i_device_get_size(self->idevice, width, height);
}

void fr_draw_context_set_surfaces(FrDrawContext* self, SysHArray* surfaces) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(surfaces != NULL);

  self->surfaces = surfaces;
}

SysHArray* fr_draw_context_get_surfaces(FrDrawContext* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->surfaces;
}

FrSurface* fr_draw_context_get_surface_by_idx(FrDrawContext* self, SysInt idx) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->surfaces->pdata[idx];
}

void fr_draw_context_frame_begin(FrDrawContext* self, FrRegion* region) {
  sys_return_if_fail(self != NULL);

  SysInt width = 0, height = 0;
  FrSurface* idevice_surface;
  FrSurface** p;

  fr_i_device_get_size(self->idevice, &width, &height);
  idevice_surface = fr_surface_create_device_surface_full(self->idevice, width, height);

  p = (FrSurface**)(self->surfaces->pdata);
  for (SysUInt i = 0; i < self->surfaces->len; i++, p++) {
    *p = fr_surface_update_surface(*p, idevice_surface, width, height);

    prepare_paint_surface(self->idevice, *p, region);
  }

  self->idevice_surface = idevice_surface;
  self->is_painting = false;
}

static void draw_context_frame_composite(FrDrawContext *self) {
  FrSurface* pb, *pn;
  FrContext *cr, *paint_cr;

  pb = self->idevice_surface;
  pn = self->surfaces->pdata[0];

  cr = fr_context_new_I(pb);
  paint_cr = fr_context_new_I(pn);

  fr_context_stoke_debug(paint_cr);

  fr_context_set_source_surface(cr, pb, 0, 0);
  fr_context_paint(cr);

  sys_object_unref(cr);
  sys_object_unref(paint_cr);

#if 0
  for (SysUInt i = 0; i < self->surfaces->len; i++) {
    pn = self->surfaces->pdata[i];

    cr = fr_context_new_I(pb);
    paint_cr = fr_context_new_I(pn);

    fr_context_stoke_debug(paint_cr);

    fr_context_overlay(cr, pb, 0, 0);
    fr_context_paint(cr);

    sys_object_unref(cr);

    pb = pn;
  }
#endif
}

void fr_draw_context_frame_end(FrDrawContext* self, FrRegion* region) {
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
  sys_clear_pointer(&cr, _sys_object_unref);

  draw_context_frame_composite(self);

  fr_surface_flush(self->idevice_surface);
  sys_clear_pointer(&self->idevice_surface, _sys_object_unref);

  self->is_painting = true;
}


/* object api */
static void fr_draw_context_construct(FrDrawContext *self, FrIDraw* iface, FrIDevice *idevice) {
  self->iface = sys_object_ref(iface);
  self->idevice = sys_object_ref(idevice);
  self->idevice_surface = NULL;
}

FrDrawContext* fr_draw_context_new(void) {
  return sys_object_new(FR_TYPE_DRAW_CONTEXT, NULL);
}

FrDrawContext *fr_draw_context_new_I(FrIDraw *iface, FrIDevice* idevice) {
  FrDrawContext *o = fr_draw_context_new();

  fr_draw_context_construct(o, iface, idevice);

  return o;
}

static void fr_draw_context_dispose(SysObject* o) {
  FrDrawContext *self = FR_DRAW_CONTEXT(o);

  sys_clear_pointer(&self->idevice, _sys_object_unref);
  sys_clear_pointer(&self->iface, _sys_object_unref);

  SYS_OBJECT_CLASS(fr_draw_context_parent_class)->dispose(o);
}

static void fr_draw_context_class_init(FrDrawContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_draw_context_dispose;
}

void fr_draw_context_init(FrDrawContext* self) {
}
