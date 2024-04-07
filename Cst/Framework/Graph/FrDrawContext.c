#include <Framework/Graph/FrDrawContext.h>
#include <Framework/Graph/FrContext.h>
#include <Framework/Graph/FrSurface.h>
#include <Framework/Graph/FrCompositor.h>
#include <Framework/Device/FrIDevice.h>


SYS_DEFINE_TYPE(FrDrawContext, fr_draw_context, SYS_TYPE_OBJECT);


SysBool fr_draw_context_frame_need_draw(FrDrawContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return !self->is_painting;
}

void fr_draw_context_set_is_painting(FrDrawContext* self,
    SysBool is_painting) {
  sys_return_if_fail(self != NULL);

  self->is_painting = is_painting;
}

SysBool fr_draw_context_get_is_painting(FrDrawContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->is_painting;
}

void fr_draw_context_get_buffer_size(FrDrawContext* self,
    SysInt* width,
    SysInt* height) {

  fr_i_device_get_size(self->idevice, width, height);
}

void fr_draw_context_add_surface(FrDrawContext* self, FrSurface *surface) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(surface != NULL);

  sys_harray_add(&self->surfaces, surface);
}

FrSurface* fr_draw_context_get_default_surface(FrDrawContext* self) {
  FrSurface* surface;
  SysUInt len;

  len = self->surfaces.len;
  if (len == 0) { return NULL; }
  surface = self->surfaces.pdata[len - 1];

  return surface;
}

SysHArray* fr_draw_context_get_surfaces(FrDrawContext* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return &self->surfaces;
}

FrSurface* fr_draw_context_get_surface_by_idx(FrDrawContext* self,
    SysInt idx) {
  sys_return_val_if_fail(self != NULL, NULL);

  if (idx >= self->surfaces.len) {
    sys_warning_N("surface index not correct: %d", idx);
    return NULL;
  }


  return self->surfaces.pdata[idx];
}

static void surface_update(FrDrawContext* self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  FrSurface* p;
  FrContext* cr;
  FrColor color = {0.0, 0.0, 1.0, 1.0};

  for (SysUInt i = 0; i < self->surfaces.len; i++) {
    p = self->surfaces.pdata[i];

    fr_surface_update_surface(p, self->idevice_surface, width, height);

    cr = fr_context_new_I(p);
    fr_context_fill_background(cr, width, height);

    fr_context_set_color(cr, &color);
    fr_context_print_text(cr, 10, 20, "中文");

    sys_clear_pointer(&cr, _sys_object_unref);
  }
}

static void surface_clip(FrContext* cr, FrRegion* region) {
  int n_boxes, i;
  FrBound box = { 0 };

  n_boxes = fr_region_num_rectangles(region);
  for (i = 0; i < n_boxes; i++) {
    fr_region_get_rectangle(region, i, &box);
    fr_context_rectangle(cr, box.x, box.y, box.width, box.height);
  }

  fr_context_rectangle(cr, box.x, box.y, box.width, box.height);
  fr_context_clip(cr);
  fr_context_paint(cr);
}

void fr_draw_context_frame_begin(FrDrawContext* self, FrRegion* region) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(region != NULL);
  SysInt width = 0, height = 0;

  fr_i_device_get_size(self->idevice, &width, &height);

  self->idevice_surface = fr_surface_create_device_surface_full(
      self->idevice,
      width, height);

  surface_update(self, width, height);

  self->is_painting = true;
}

static void surface_composite(FrDrawContext* self) {
  sys_return_if_fail(self != NULL);
  FrSurface* pb, * pn;
  FrContext* cr;
  SysHArray* paint_surfaces = &self->surfaces;

  if (paint_surfaces->len < 1) { return; }

  for (SysInt i = paint_surfaces->len - 1; i > 0; i--) {
    pn = paint_surfaces->pdata[i];
    pb = paint_surfaces->pdata[i - 1];

    cr = fr_context_new_I(pb);
    fr_compositor_surface_overlay(pn, cr);
    sys_clear_pointer(&cr, _sys_object_unref);
  }

  pn = paint_surfaces->pdata[0];
  pb = self->idevice_surface;

  cr = fr_context_new_I(pb);
  fr_compositor_surface_overlay(pn, cr);
  sys_object_unref(cr);
}

void fr_draw_context_frame_end(FrDrawContext* self, FrRegion* region) {
  sys_return_if_fail(self != NULL);

  surface_composite(self);

  fr_surface_flush(self->idevice_surface);
  sys_clear_pointer(&self->idevice_surface, _sys_object_unref);

  self->is_painting = false;
}

/* object api */
static void fr_draw_context_construct(FrDrawContext *self, FrIDraw* iface,
    FrIDevice *idevice) {
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
  sys_harray_destroy(&self->surfaces);

  SYS_OBJECT_CLASS(fr_draw_context_parent_class)->dispose(o);
}

static void fr_draw_context_class_init(FrDrawContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_draw_context_dispose;
}

void fr_draw_context_init(FrDrawContext* self) {

  sys_harray_init_with_free_func(&self->surfaces, (SysDestroyFunc)_sys_object_unref);
}
