#include <CstCore/Driver/CstAbsNode.h>

SYS_DEFINE_TYPE(CstAbsNode, cst_abs_node, CST_TYPE_LAYER_NODE);

CstLayerNode *cst_abs_node_get_children_i(CstLayerNode *o) {
  CstAbsNode *anode = CST_ABS_NODE(o);
  CstLayerNode *layer = cst_layer_node_get_layer(o);

  return cst_abs_layer_get_parent(layer, anode);
}

/* object api */
static void cst_abs_node_construct(CstLayerNode *o, CstLayer *layer) {

  CST_LAYER_NODE_CLASS(cst_abs_node_parent_class)->construct(o, layer);
}

CstLayerNode* cst_abs_node_new(void) {
  return sys_object_new(CST_TYPE_ABS_NODE, NULL);
}

CstLayerNode*cst_abs_node_new_I(CstLayer *layer) {
  CstLayerNode *o = cst_abs_node_new();

  cst_abs_node_construct(o, layer);

  return o;
}

static void cst_abs_node_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(cst_abs_node_parent_class)->dispose(o);
}

static void cst_abs_node_class_init(CstAbsNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstLayerNodeClass *lcls = CST_LAYER_NODE_CLASS(cls);

  ocls->dispose = cst_abs_node_dispose;
  lcls->get_children = cst_abs_node_get_children_i;
  lcls->get_parent = cst_abs_node_get_parent_i;
}

void cst_abs_node_init(CstAbsNode* self) {
}

