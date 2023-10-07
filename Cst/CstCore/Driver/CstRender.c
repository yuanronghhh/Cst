#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Front/CstComponent.h>
#include <CstCore/Front/Common/CstLBody.h>
#include <CstCore/Driver/CstRenderContext.h>

SYS_DEFINE_TYPE(CstRender, cst_render, SYS_TYPE_OBJECT);

CstRender *cst_render_new(void) {
  return sys_object_new(CST_TYPE_RENDER, NULL);
}

FRWindow *cst_render_get_default_window(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->window;
}

CstBoxLayer *cst_render_get_box_layer(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->box_layer;
}

CstAbsLayer *cst_render_get_abs_layer(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->abs_layer;
}

CstNode *cst_render_get_root(CstRender* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->root;
}

FRRegion *render_create_region(FRWindow *window) {
  FRRegion *region;
  FRRect bound = { 0 };

  fr_window_get_framebuffer_size(window, &bound.width, &bound.height);
  region = fr_region_create_rectangle(&bound);

  return region;
}

void cst_render_render(CstRender *self) {
  FRRegion *region = render_create_region(self->window);

  cst_render_rerender(self, region);

  fr_region_destroy(region);
}

void cst_render_rerender(CstRender *self, FRRegion *region) {
  sys_return_if_fail(self != NULL);

  FRDraw *draw = fr_draw_new_I(self->window);
  CstLayout *layout = cst_layout_new_I(draw, region);

  cst_box_layer_check(self->box_layer, layout);
  cst_box_layer_layout(self->box_layer, layout);
  cst_box_layer_render(self->box_layer, layout);

  // cst_layer_check(self->abs_layer, layout);
  // cst_layer_layout(self->abs_layer, layout);
  // cst_layer_render(self->abs_layer, draw, region);

  sys_object_unref(draw);
}

void cst_render_resize_window(CstRender *self) {
  sys_return_if_fail(self != NULL);

  SysInt width = 0;
  SysInt height = 0;

  fr_window_get_size(self->window, &width, &height);

  cst_render_request_resize_window(self, width, height);
}

void cst_render_request_resize_window(CstRender *self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  FRRegion *region;
  FRRect bound = { 0 };

  bound.width = width;
  bound.height = height;

  region = fr_region_create_rectangle(&bound);

  cst_render_rerender(self, region);

  fr_region_destroy(region);
}


/* object api */
static void cst_render_construct(CstRender *self, SysBool is_offscreen) {

  if (is_offscreen) {

    self->window = NULL;
  } else {

    self->display = fr_display_new_I();
    self->window = fr_window_top_new(self->display);
  }

  self->box_layer = (CstBoxLayer *)cst_box_layer_new_I();
  self->abs_layer = (CstAbsLayer *)cst_abs_layer_new_I();
  self->root = cst_lbody_new();
}

CstRender* cst_render_new_I(SysBool is_offscreen) {
  CstRender* o = cst_render_new();

  cst_render_construct(o, is_offscreen);

  return o;
}

static void cst_render_init(CstRender *self) {
}

static void cst_render_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  CstRender *self = CST_RENDER(o);

  sys_object_unref(self->box_layer);
  sys_object_unref(self->abs_layer);

  if (self->window) {
    sys_object_unref(self->window);
    sys_object_unref(self->display);
  }

  SYS_OBJECT_CLASS(cst_render_parent_class)->dispose(o);
}

static void cst_render_class_init(CstRenderClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_render_dispose;
}
