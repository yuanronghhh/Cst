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

FrSurface *fr_draw_get_paint_surface(FrDraw *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->paint_surface;
}

void fr_draw_frame_begin(FrDraw *self, FrRegion *region) {
  sys_return_if_fail(self != NULL);
  SysInt width = 0, height = 0;

  fr_i_device_get_size(self->idevice, &width, &height);

  self->idevice_surface = fr_surface_create_device_surface_full(self->idevice, width, height);
  self->paint_surface = fr_surface_create_image_surface_from_surface(self->idevice_surface, width, height);

  sys_assert(self->ctx == NULL && "draw cr should be NULL when cairo_frame_begin_i, missing fr_cairo_draw_frame_end ?");

  self->ctx = fr_context_new_I(self->paint_surface);
  fr_context_fill_background(self->ctx, width, height);

  self->is_painting = true;
}

void fr_draw_frame_end(FrDraw *self, FrRegion *region) {
  sys_return_if_fail(self != NULL);

  int n_boxes, i;
  cairo_rectangle_int_t box;
  FrContext* ctx;

  ctx = fr_context_new_I(self->idevice_surface);
  fr_context_set_source_surface(ctx, self->paint_surface, 0, 0);

  n_boxes = cairo_region_num_rectangles(region);
  for (i = 0; i < n_boxes; i++) {
    fr_region_get_rectangle(region, i, &box);
    fr_context_rectangle(ctx, box.x, box.y, box.width, box.height);
  }

  fr_context_rectangle(ctx, box.x, box.y, box.width, box.height);
  fr_context_clip(ctx);
  fr_context_paint(ctx);
  sys_object_unref(ctx);

  sys_clear_pointer(&self->ctx, fr_context_destroy);
  fr_surface_flush(self->idevice_surface);

  sys_clear_pointer(&self->idevice_surface, _sys_object_unref);
  sys_clear_pointer(&self->paint_surface, _sys_object_unref);

  self->is_painting = false;
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
