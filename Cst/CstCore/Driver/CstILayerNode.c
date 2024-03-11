#include <CstCore/Driver/CstILayerNode.h>


SYS_DEFINE_INTERFACE(CstILayerNode, cst_ilayer_node, SYS_TYPE_OBJECT);

/* object api */
void cst_ilayer_node_default_init(CstILayerNodeInterface* iface) {
}

CstILayerNode* cst_ilayer_node_get_children (CstILayerNode *item) {
  sys_return_val_if_fail(item != NULL, NULL);

  return CST_ILAYER_NODE_GET_IFACE(item)->get_children(item);
}
