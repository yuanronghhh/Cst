#include <CstCore/Driver/CstLayer.h>
#include <CstCore/Driver/CstLayout.h>

SYS_DEFINE_TYPE(CstLayer, cst_layer, SYS_TYPE_OBJECT);

void cst_layer_queue_draw_node(CstLayer *self, CstRenderNode *render_node) {
  sys_return_if_fail(self != NULL);

  sys_object_ref(render_node);
  sys_queue_push_tail(self->draw_queue, render_node);
}

CstLayerNode* cst_layer_realize_node (CstLayer *self, CstLayerNode *parent, CstRenderNode *rnode) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstLayerClass* lcls = CST_LAYER_GET_CLASS(self);
  sys_return_val_if_fail(lcls->realize_node != NULL, NULL);

  return lcls->realize_node(self, parent, rnode);
}

CstLayerNode* cst_layer_realize_node_i (CstLayer *o, CstLayerNode *parent, CstRenderNode *rnode) {
  return NULL;
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
}

static void cst_layer_init(CstLayer *self) {
  self->draw_queue = sys_queue_new();
}
