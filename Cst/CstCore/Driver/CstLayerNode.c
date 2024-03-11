#include <CstCore/Driver/CstLayerNode.h>

#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstILayerNode.h>

static void cst_ilayer_node_imp(CstILayerNodeInterface* iface);

SYS_DEFINE_WITH_CODE(CstLayerNode, cst_layer_node, SYS_TYPE_OBJECT,
    SYS_IMPLEMENT_INTERFACE(CST_TYPE_ILAYER_NODE, cst_ilayer_node_imp));


static void cst_ilayer_node_imp(CstILayerNodeInterface* iface) {
}

CstRenderNode * cst_layer_node_get_render_node(CstLayerNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return CST_RENDER_NODE(self);
}

/* object api */
static void cst_layer_node_dispose(SysObject* o) {
  CstLayerNode *self = CST_LAYER_NODE(o);

  SYS_OBJECT_CLASS(cst_layer_node_parent_class)->dispose(o);
}

static void cst_layer_node_construct(CstLayerNode* self, CstLayer *layer, CstNode *node) {
  // self->node = node;
  self->layer = layer;

  sys_object_ref(node);
}

CstLayerNode *cst_layer_node_new(void) {
  return sys_object_new(CST_TYPE_LAYER_NODE, NULL);
}

CstLayerNode *cst_layer_node_new_I(CstLayer *layer, CstNode *node) {
  CstLayerNode *o = cst_layer_node_new();

  cst_layer_node_construct(o, layer, node);

  return o;
}

static void cst_layer_node_class_init(CstLayerNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layer_node_dispose;
  cls->construct = cst_layer_node_construct;
}

static void cst_layer_node_init(CstLayerNode *self) {
}
