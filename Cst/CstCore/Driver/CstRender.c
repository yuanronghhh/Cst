#include <CstCore/Driver/CstRender.h>
#include <CstCore/Front/Common/CstComponent.h>
#include <CstCore/Front/Common/CstLBody.h>
#include <CstCore/Front/Common/CstWidget.h>


struct _CstRenderPrivate {
  FRDisplay *display;
  FRWindow *window;

  FRDraw *draw;
  SysQueue *draw_queue;

  CstLayer *box_layer;
  CstLayer *abs_layer;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstRender, cst_render, SYS_TYPE_OBJECT);

CstRender *cst_render_new(void) {
  return sys_object_new(CST_TYPE_RENDER, NULL);
}

void cst_render_get_size(CstRender *self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);

  CstRenderPrivate *priv = self->priv;

  SysInt w = 0, h = 0;

  if (priv->window) {
    fr_window_get_size(priv->window, &w, &h);
  }

  *width = w;
  *height = h;
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

void cst_render_render(CstModule *v_module, CstRender *self) {
  sys_return_if_fail(self != NULL);

  CstRenderPrivate *priv = self->priv;

  cst_layer_render(priv->box_layer, v_module, self);
}

FRContext *cst_render_get_cr(CstRender *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstRenderPrivate *priv = self->priv;

  return fr_draw_get_cr(priv->draw);
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

FRRegion * cst_render_create_region(CstRender *self, const FRRect *n_rect) {
  sys_return_val_if_fail(self != NULL, NULL);

  return fr_region_create_rectangle(n_rect);
}

void cst_render_frame_begin(CstRender *self, FRRegion *region) {
  sys_return_if_fail(self != NULL);

  CstRenderPrivate *priv = self->priv;

  fr_draw_frame_begin(priv->draw, region);
}

void cst_render_frame_end(CstRender *self, FRRegion *region) {
  sys_return_if_fail(self != NULL);

  CstRenderPrivate *priv = self->priv;

  fr_draw_frame_end(priv->draw, region);
}

void cst_render_request_resize_window(CstRender *self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  FRRect bound = { 0 };
  CstRenderPrivate *priv = self->priv;

  bound.width = width;
  bound.height = height;

  fr_window_set_size(priv->window, width, height);

  cst_render_request_redraw(self, &bound);
}

void cst_render_resize_window(CstRender *self) {
  sys_return_if_fail(self != NULL);

  FRRect bound = { 0 };
  CstRenderPrivate *priv = self->priv;
  CstNode *root = cst_box_layer_get_root(CST_BOX_LAYER(priv->box_layer));

  fr_window_get_size(priv->window, &(bound.width), &(bound.height));

  cst_node_set_prefer_size(root, bound.width, bound.height);
  cst_node_set_size(root, bound.width, bound.height);

  cst_render_request_redraw(self, &bound);
}

void cst_render_rerender(CstRender *self) {
  sys_return_if_fail(self != NULL);
  CstRenderPrivate *priv = self->priv;

  if (sys_queue_get_length(priv->draw_queue) > 0) {
  }
}

void cst_render_request_redraw(CstRender *self, FRRect *bound) {
  sys_return_if_fail(self != NULL);
  CstRenderPrivate *priv = self->priv;

  cst_layer_check(priv->box_layer, self, bound);
  cst_layer_check(priv->abs_layer, self, bound);

  cst_render_rerender(self);
}

void cst_render_queue_draw_node(CstRender *self, CstNode *node) {
  sys_return_if_fail(self != NULL);

  CstRenderPrivate *priv = self->priv;

  sys_queue_push_head(priv->draw_queue, node);
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

  priv->draw = fr_canvas_create_draw(priv->window);
  priv->draw_queue = sys_queue_new();

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

  sys_clear_pointer(&priv->draw, fr_draw_destroy);
  sys_queue_free_full(priv->draw_queue, (SysDestroyFunc)_sys_object_unref);

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
