#include <CstCore/Driver/CstILayerNode.h>

SYS_DEFINE_INTERFACE(CstILayerNode, cst_i_layer_node, SYS_TYPE_OBJECT);

/* object api */
void cst_i_layer_node_default_init(CstILayerNodeInterface* iface) {
}

void cst_i_layer_node_iterate_node (CstLayerNode *self, CstLayerNodeFunc func, SysPointer user_data) {
  sys_return_if_fail(self != NULL);

  CST_I_LAYER_NODE_GET_IFACE(self)->iterate_node(self, func, user_data);
}

CstLayerNode * cst_i_layer_node_get_children (CstLayerNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return CST_I_LAYER_NODE_GET_IFACE(self)->get_children(self);
}

CstLayerNode * cst_i_layer_node_get_parent (CstLayerNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return CST_I_LAYER_NODE_GET_IFACE(self)->get_parent(self);
}

CstLayerNode * cst_i_layer_node_get_next (CstLayerNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return CST_I_LAYER_NODE_GET_IFACE(self)->get_next(self);
}

CstLayer * cst_i_layer_node_get_layer (CstLayerNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return CST_I_LAYER_NODE_GET_IFACE(self)->get_layer(self);
}
