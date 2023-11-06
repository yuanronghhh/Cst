#include <CstCore/Driver/CstRender.h>

#include <CstCore/Front/Common/CstLBody.h>
#include <CstCore/Driver/CstNodeBuilder.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstRenderContext.h>


typedef struct _CstRenderPass CstRenderPass;

struct _CstRenderPass {
  CstModule *v_module;
};

SYS_DEFINE_TYPE(CstRender, cst_render, SYS_TYPE_OBJECT);


CstRender *cst_render_new(void) {
  return sys_object_new(CST_TYPE_RENDER, NULL);
}

FRWindow *cst_render_get_default_window(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->window;
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

  CstLayer *layer;

  layer = self->box_layer;
  cst_layout_begin_layout(layout, layer);

  cst_box_layer_check(layer, layout);
  cst_box_layer_layout(layer, layout);
  cst_box_layer_render(layer, layout);

  cst_layout_end_layout(layout);
}

void cst_render_realize(CstRender *self, CstRenderPass *ctx) {
  sys_return_if_fail(self != NULL);

  CstLayerNode* body;
  FRRegion* region;
  CstModule *v_module;
  FRRect bound = { 0 };

  v_module= ctx->v_module;

  fr_window_get_framebuffer_size(self->window, &(bound.width), &(bound.height));
  region = fr_region_create_rectangle(&bound);

  body = cst_node_realize(cst_node_get_body_node(), NULL, NULL);
  cst_module_realize(v_module, body);
  fr_region_destroy(region);

  cst_box_layer_set_root(CST_BOX_LAYER(self->box_layer), CST_BOX_NODE(body));
}

void cst_render_render(CstRender *self, CstModule *v_module) {
  sys_return_if_fail(self != NULL);

  CstLayer *layer;
  FRRegion *region;
  CstLayout* layout;

  CstRenderPass ctx = {0};
  ctx.v_module = v_module;

  region = render_create_region(self->window);
  layout = cst_layout_new_I(self, region);
  layer = self->box_layer;

  cst_render_realize(self, &ctx);
  cst_layout_begin_layout(layout, layer);

  cst_box_layer_layout(layer, layout);
  cst_box_layer_render(layer, layout);

  cst_layout_end_layout(layout);
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

  CstLayout* layout = cst_layout_new_I(self, region);
  cst_render_rerender(self, region, layout);

  sys_object_unref(layout);
  fr_region_destroy(region);
}

CstLayer *cst_render_get_layer_by_type(CstRender *self, SysInt layer_type) {
  sys_return_val_if_fail(self != NULL, NULL);

  switch(layer_type) {
    case CST_NODE_LAYER_BOX:
      return self->box_layer;
    case CST_NODE_LAYER_ABS:
      return self->abs_layer;
    case CST_NODE_LAYER_PASS:
    default:
      sys_warning_N("layer not implement: %d", layer_type);
      break;
  }

  return NULL;
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
