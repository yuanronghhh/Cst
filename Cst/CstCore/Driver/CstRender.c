#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstNodeBuilder.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstComponent.h>
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

CstLayer *cst_render_get_box_layer(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->box_layer;
}

CstLayer *cst_render_get_abs_layer(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->abs_layer;
}

void cst_render_set_layer_root(CstRender* self, CstRenderNode *root) {
  sys_return_if_fail(self != NULL);

  cst_box_layer_set_root(CST_BOX_LAYER(self->box_layer), CST_BOX_NODE(root));
}

CstNode *cst_render_get_body_node(CstRender* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->body_node;
}

CstRenderNode* cst_render_render_node_new_root(CstRender *self, CstModule *v_module) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(self->body_node != NULL, NULL);

  CstRenderNode* bnode = cst_box_node_new_I(self->body_node);

  return bnode;
}

FRRegion *render_create_region(FRWindow *window) {
  FRRegion *region;
  FRRect bound = { 0 };

  fr_window_get_framebuffer_size(window, &bound.width, &bound.height);
  region = fr_region_create_rectangle(&bound);

  return region;
}

void cst_render_rerender(CstRender* self, FRRegion* region, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  cst_layout_begin_layout(layout, CST_LAYER_BOX);

  cst_box_layer_check(self->box_layer, layout);
  cst_box_layer_layout(self->box_layer, layout);
  cst_box_layer_render(self->box_layer, layout);

  cst_layout_end_layout(layout);
}

void cst_render_render(CstRender *self) {
  sys_return_if_fail(self != NULL);

  FRRegion *region = render_create_region(self->window);
  CstLayout* layout = cst_layout_new_I(self->draw, region);

  cst_layout_begin_layout(layout, CST_LAYER_BOX);
  cst_box_layer_layout(self->box_layer, layout);
  cst_layout_end_layout(layout);

  cst_box_layer_render(self->box_layer, layout);

  fr_region_destroy(region);

  sys_object_unref(layout);
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

  CstLayout* layout = cst_layout_new_I(self->draw, region);
  cst_render_rerender(self, region, layout);

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

  self->box_layer = cst_box_layer_new_I();
  self->abs_layer = cst_abs_layer_new_I();
  self->body_node = cst_lbody_new();
  self->draw = fr_draw_new_I(self->window);

  CstNodeBuilder* builder = cst_node_builder_new();
  cst_node_construct(self->body_node, builder);
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

  sys_clear_pointer(&self->box_layer, _sys_object_unref);
  sys_clear_pointer(&self->abs_layer, _sys_object_unref);
  sys_clear_pointer(&self->draw, _sys_object_unref);

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
