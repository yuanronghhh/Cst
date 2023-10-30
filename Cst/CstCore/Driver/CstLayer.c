#include <CstCore/Driver/CstLayer.h>

#include <CstCore/Driver/CstLayerNode.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstRenderNode.h>

SYS_DEFINE_TYPE(CstLayer, cst_layer, SYS_TYPE_OBJECT);

void cst_layer_queue_draw_node(CstLayer *self, CstRenderNode *render_node) {
  sys_return_if_fail(self != NULL);

  sys_object_ref(render_node);
  sys_queue_push_tail(self->draw_queue, render_node);
}

CstLayerNode* cst_layer_realize_node (CstLayer *self, CstLayerNode *parent, CstNode *node, CstLayout *layout) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstLayerClass* lcls = CST_LAYER_GET_CLASS(self);
  sys_return_val_if_fail(lcls->realize_node != NULL, NULL);

  return lcls->realize_node(self, parent, node, layout);
}

CstLayerNode* cst_layer_new_node(CstLayer *self, CstNode *node) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstLayerClass* lcls = CST_LAYER_GET_CLASS(self);
  sys_return_val_if_fail(lcls->new_node != NULL, NULL);

  return lcls->new_node(self, node);
}

CstLayerNode* cst_layer_new_node_i(CstLayer *self, CstNode *node) {
  sys_return_val_if_fail(self != NULL, NULL);

  return cst_layer_node_new_I(node);
}

CstLayerNode* cst_layer_realize_node_i (CstLayer *o, CstLayerNode *parent, CstNode *node, CstLayout *layout) {
  CstRenderNode *rnode;
  CstLayerNode *lnode;

  CstRender *v_render = cst_layout_get_render(layout);
  sys_return_val_if_fail(v_render != NULL, NULL);

  rnode = cst_node_new_render_node(node);
  lnode = cst_node_new_layer_node(node, v_render);
  cst_render_node_set_layer_node(rnode, lnode);

  return lnode;
}

/* object api */
static void cst_layer_dispose(SysObject* o) {
  CstLayer *self = CST_LAYER(o);

  sys_queue_free_full(self->draw_queue, (SysDestroyFunc)_sys_object_unref);

  SYS_OBJECT_CLASS(cst_layer_parent_class)->dispose(o);
}

CstLayer *cst_layer_new(void) {
  return sys_object_new(CST_TYPE_LAYER, NULL);
}

static void cst_layer_class_init(CstLayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layer_dispose;
  cls->realize_node = cst_layer_realize_node_i;
  cls->new_node = cst_layer_new_node_i;
}

static void cst_layer_init(CstLayer *self) {
  self->draw_queue = sys_queue_new();
}
