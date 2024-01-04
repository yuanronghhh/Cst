#include <CstCore/Driver/CstLayer.h>

#include <CstCore/Driver/CstLayerNode.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstRenderNode.h>

SYS_DEFINE_TYPE(CstLayer, cst_layer, SYS_TYPE_OBJECT);

static const SysChar* CST_NODE_LAYER_NAMES[] = {
  "box", "absolute"
};

CST_NODE_LAYER_ENUM cst_layer_get_by_prop(const SysChar* name) {
  return fr_get_type_by_name(CST_NODE_LAYER_NAMES, ARRAY_SIZE(CST_NODE_LAYER_NAMES), name);
}

void cst_layer_set_name(CstLayer *self, const SysChar* name) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(name != NULL);

  self->name = sys_strdup(name);
}

void cst_layer_queue_draw_node(CstLayer *self, CstLayoutNode *lynode) {
  sys_return_if_fail(self != NULL);

  sys_object_ref(lynode);
  sys_queue_push_tail(self->draw_queue, lynode);
}

CstLayerNode* cst_layer_new_node(CstLayer *self, CstLayerNode *parent, CstNode *node) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstLayerClass* lcls = CST_LAYER_GET_CLASS(self);
  sys_return_val_if_fail(lcls->new_node != NULL, NULL);

  return lcls->new_node(self, parent, node);
}

CstLayerNode* cst_layer_new_node_i(CstLayer *self, CstLayerNode *parent, CstNode *node) {
  sys_return_val_if_fail(self != NULL, NULL);

  return cst_layer_node_new_I(node);
}

/* object api */
static void cst_layer_dispose(SysObject* o) {
  CstLayer *self = CST_LAYER(o);

  sys_queue_free_full(self->draw_queue, (SysDestroyFunc)_sys_object_unref);
  sys_clear_pointer(&self->name, sys_free);

  SYS_OBJECT_CLASS(cst_layer_parent_class)->dispose(o);
}

CstLayer *cst_layer_new(void) {
  return sys_object_new(CST_TYPE_LAYER, NULL);
}

static void cst_layer_class_init(CstLayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layer_dispose;
  cls->new_node = cst_layer_new_node_i;
}

static void cst_layer_init(CstLayer *self) {
  self->draw_queue = sys_queue_new();
}
