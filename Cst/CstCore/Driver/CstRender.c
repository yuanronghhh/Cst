#include <CstCore/Driver/CstRender.h>
#include <CstCore/Front/Common/CstComponent.h>
#include <CstCore/Front/Common/CstLBody.h>
#include <CstCore/Front/Common/CstWidget.h>


struct _CstRenderPrivate {
  FRDisplay *display;
  FRWindow *window;

  FRDraw *draw;

  CstLayer *box_layer;
  CstLayer *abs_layer;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstRender, cst_render, SYS_TYPE_OBJECT);

CstRender *cst_render_new(void) {
  return sys_object_new(CST_TYPE_RENDER, NULL);
}

FRDraw *cst_render_get_draw(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);
  CstRenderPrivate *priv = self->priv;

  return priv->draw;
}

CstBoxLayer *cst_render_get_box_layer(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstRenderPrivate *priv = self->priv;

  return CST_BOX_LAYER(priv->box_layer);
}

CstAbsLayer *cst_render_get_abs_layer(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstRenderPrivate *priv = self->priv;

  return CST_ABS_LAYER(priv->abs_layer);
}

void cst_render_render(CstRender *self) {
  sys_return_if_fail(self != NULL);

  CstRenderPrivate *priv = self->priv;

  FRRect bound;
  FRRegion *region;
  FRDraw *draw = priv->draw;
  FRContext *cr;

  fr_window_get_size(priv->window, &bound.width, &bound.height);
  region = fr_region_create_rectangle(&bound);

  fr_draw_frame_begin(draw, region);

  cr = fr_draw_create_cr(draw);
  cst_layer_render(priv->box_layer, draw, cr);
  cst_layer_render(priv->abs_layer, draw, cr);
  fr_context_destroy(cr);

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
  CstRenderPrivate *priv = self->priv;

  fr_window_get_size(priv->window, &width, &height);

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

  CstRenderPrivate *priv = self->priv;
  FRDraw *draw = priv->draw;
  FRContext *cr;

  if (fr_draw_frame_need_draw(draw)) {
    fr_draw_frame_begin(draw, region);

    cr = fr_draw_create_cr(draw);

    cst_layer_check(priv->box_layer, draw, region);
    cst_layer_check(priv->abs_layer, draw, region);

    cst_layer_rerender(priv->box_layer, draw, cr);
    cst_layer_rerender(priv->abs_layer, draw, cr);
    
    fr_context_destroy(cr);

    fr_draw_frame_end(draw, region);
  }
}

/* object api */
void cst_render_construct(SysObject *o, SysBool is_offscreen) {
  SYS_OBJECT_CLASS(cst_render_parent_class)->construct(o);

  CstRender *self = CST_RENDER(o);
  CstRenderPrivate *priv = self->priv;

  if (is_offscreen) {

    priv->window = NULL;
  } else {

    priv->display = fr_display_new_I();
    priv->window = fr_window_top_new(priv->display);
  }

  priv->draw = fr_draw_new_I(priv->window);

  priv->box_layer = cst_box_layer_new_I();
  priv->abs_layer = cst_abs_layer_new_I();
}

CstRender* cst_render_new_I(SysBool is_offscreen) {
  CstRender* o = cst_render_new();

  cst_render_construct(SYS_OBJECT(o), is_offscreen);

  return o;
}

static void cst_render_init(CstRender *self) {
  self->priv = cst_render_get_private(self);
}

static void cst_render_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  CstRender *self = CST_RENDER(o);
  CstRenderPrivate *priv = self->priv;

  sys_object_unref(priv->box_layer);
  sys_object_unref(priv->abs_layer);

  sys_clear_pointer(&priv->draw, _sys_object_unref);

  if (priv->window) {
    sys_object_unref(priv->window);
    sys_object_unref(priv->display);
  }

  SYS_OBJECT_CLASS(cst_render_parent_class)->dispose(o);
}

static void cst_render_class_init(CstRenderClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)cst_render_construct;
  ocls->dispose = cst_render_dispose;
}
