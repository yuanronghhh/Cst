#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Front/Common/CstComponent.h>
#include <CstCore/Front/Common/CstLBody.h>
#include <CstCore/Front/Common/CstWidget.h>

SYS_DEFINE_TYPE(CstRender, cst_render, SYS_TYPE_OBJECT);

CstRender *cst_render_new(void) {
  return sys_object_new(CST_TYPE_RENDER, NULL);
}

FRDraw *cst_render_get_draw(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->draw;
}

FRWindow *cst_render_get_default_window(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->window;
}

CstBoxLayer *cst_render_get_box_layer(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return CST_BOX_LAYER(self->box_layer);
}

CstAbsLayer *cst_render_get_abs_layer(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return CST_ABS_LAYER(self->abs_layer);
}

void cst_render_render(CstRender *self) {
  sys_return_if_fail(self != NULL);

  FRRegion *region;

  FRRect bound = { 0 };
  FRDraw *draw = self->draw;
  CstLayout *layout = self->layout;

  sys_assert(draw != NULL && "draw must init before render use");

  fr_window_get_framebuffer_size(self->window, &bound.width, &bound.height);
  region = fr_region_create_rectangle(&bound);

  fr_draw_frame_begin(draw, region);

  cst_layer_render(self->box_layer, draw, layout);
  cst_layer_render(self->abs_layer, draw, layout);

  fr_draw_frame_end(draw, region);

  fr_region_destroy(region);
}

void cst_render_set_node(CstRender *self, CstNode *parent, CstNode *node) {
  sys_return_if_fail(node != NULL);
  sys_return_if_fail(parent != NULL);

  CstNode *last_child;
  CstBoxLayer *box_layer = cst_render_get_box_layer(self);

  last_child = cst_node_get_last_child(parent);

  cst_box_layer_insert_after(box_layer, parent, last_child, node);

  cst_node_set_last_child(parent, node);
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

void cst_render_rerender(CstRender *self, FRRegion *region) {
  sys_return_if_fail(self != NULL);

  FRDraw *draw = self->draw;
  CstLayout *layout = self->layout;

  if (!fr_draw_frame_need_draw(draw)) {
    return;
  }

  fr_draw_frame_begin(draw, region);

  cst_layer_check(self->box_layer, draw, region);
  cst_layer_check(self->abs_layer, draw, region);

  cst_layer_rerender(self->box_layer, draw, layout);
  cst_layer_rerender(self->abs_layer, draw, layout);

  fr_draw_frame_end(draw, region);
}

/* object api */
static void cst_render_construct(CstRender *self, SysBool is_offscreen) {

  if (is_offscreen) {

    self->window = NULL;
  } else {

    self->display = fr_display_new_I();
    self->window = fr_window_top_new(self->display);
  }

  self->draw = fr_draw_new_I(self->window);
  self->layout = cst_layout_new_I();

  self->box_layer = cst_box_layer_new_I();
  self->abs_layer = cst_abs_layer_new_I();
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

  sys_clear_pointer(&self->draw, _sys_object_unref);
  sys_clear_pointer(&self->layout, _sys_object_unref);

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
