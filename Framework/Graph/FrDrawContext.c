#include <Framework/Graph/FrDrawContext.h>
#include <Framework/Graph/FrCairoDrawContext.h>
#include <Framework/Graph/FrContext.h>
#include <Framework/Graph/FrSurface.h>
#include <Framework/Device/FrIDevice.h>
#include <Framework/Device/FrDevice.h>
#include <Framework/Graph/FrIDraw.h>

static FrDrawContext* g_draw = NULL;

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

  fr_i_device_get_size(FR_I_DEVICE(self->device), width, height);
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
    SysUInt idx) {
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
  FrIDrawInterface *g_draw_iface = fr_i_draw_get_g_iface();

  for (SysUInt i = 0; i < self->surfaces.len; i++) {
    p = self->surfaces.pdata[i];

    g_draw_iface->resize_surface(p, self->device_surface, width, height);

    cr = fr_context_new_I(p);

    g_draw_iface->set_color(cr, &color);
    g_draw_iface->paint(cr);

    g_draw_iface->set_color(cr, &color);
    fr_context_show_text_p(cr, 10, 20, "中文");

    sys_clear_pointer(&cr, _sys_object_unref);
  }
}

static void surface_clip(FrContext* cr, FrRegion* region) {
  int n_boxes, i;
  FrBound box = { 0 };
  FrIDrawInterface *g_draw_iface = fr_i_draw_get_g_iface();

  n_boxes = fr_region_num_rectangles(region);
  for (i = 0; i < n_boxes; i++) {
    fr_region_get_rectangle(region, i, &box);
    g_draw_iface->rectangle(cr, box.x, box.y, box.width, box.height);
  }

  g_draw_iface->rectangle(cr, box.x, box.y, box.width, box.height);
  g_draw_iface->clip(cr);
  g_draw_iface->paint(cr);
}

void fr_draw_context_frame_begin(FrDrawContext* self, FrRegion* region) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(region != NULL);
  SysInt width = 0, height = 0;

  fr_i_device_get_size(FR_I_DEVICE(self->device), &width, &height);

  FrSurfaceContext info = {.width = width, .height = height };
  self->device_surface = fr_device_create_surface(self->device, &info);

  // surface_update(self, width, height);

  self->is_painting = true;
}

static void surface_composite(FrDrawContext* self) {
  sys_return_if_fail(self != NULL);
  FrSurface* pb, * pn;
  FrContext* cr;
  SysHArray* paint_surfaces = &self->surfaces;
  FrIDrawInterface *g_draw_iface = fr_i_draw_get_g_iface();

  if (paint_surfaces->len < 1) { return; }

  for (SysInt i = paint_surfaces->len - 1; i > 0; i--) {
    pn = paint_surfaces->pdata[i];
    pb = paint_surfaces->pdata[i - 1];

    cr = fr_context_new_I(pb);
    g_draw_iface->overlay(cr, pn, 0, 0);
    sys_clear_pointer(&cr, _sys_object_unref);
  }

  pn = paint_surfaces->pdata[0];
  pb = self->device_surface;

  cr = fr_context_new_I(pb);
  g_draw_iface->overlay(cr, pn, 0, 0);
  sys_object_unref(cr);
}

void fr_draw_context_frame_end(FrDrawContext* self, FrRegion* region) {
  sys_return_if_fail(self != NULL);
  FrIDrawInterface *g_draw_iface = fr_i_draw_get_g_iface();

  // surface_composite(self);

  g_draw_iface->surface_flush(self->device_surface);
  sys_clear_pointer(&self->device_surface, _sys_object_unref);

  self->is_painting = false;
}

void fr_draw_context_construct(FrDrawContext* self, FrDevice* device) {
  sys_return_if_fail(self != NULL);

  FrDrawContextClass* cls = FR_DRAW_CONTEXT_GET_CLASS(self);

  sys_return_if_fail(cls->construct);

  cls->construct(self, device);
}

void fr_draw_context_g_set(FrDrawContext *ctx) {
  sys_assert(g_draw == NULL && "can not set draw context twice");

  g_draw = sys_object_ref(ctx);
  fr_i_draw_setup(FR_I_DRAW(g_draw));
}

void fr_draw_context_g_unset(void) {
  sys_assert(g_draw != NULL);

  sys_clear_pointer(&g_draw, _sys_object_unref);
  fr_i_draw_teardown();
}

/* object api */
static void fr_draw_context_construct_i(FrDrawContext *self, FrDevice *device) {

  self->device = device ? sys_object_ref(device) : NULL;
  self->device_surface = NULL;
}

FrDrawContext* fr_draw_context_new(void) {
  return sys_object_new(FR_TYPE_DRAW_CONTEXT, NULL);
}

static void fr_draw_context_dispose(SysObject* o) {
  FrDrawContext *self = FR_DRAW_CONTEXT(o);

  if(self->device) {

    sys_clear_pointer(&self->device, _sys_object_unref);
  }
  sys_harray_destroy(&self->surfaces);

  SYS_OBJECT_CLASS(fr_draw_context_parent_class)->dispose(o);
}

static void fr_draw_context_class_init(FrDrawContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_draw_context_construct_i;
  ocls->dispose = fr_draw_context_dispose;
}

void fr_draw_context_init(FrDrawContext* self) {

  sys_harray_init_with_free_func(&self->surfaces, (SysDestroyFunc)_sys_object_unref);
}
