#include <CstCore/Front/Common/CstIComNode.h>

SYS_DEFINE_INTERFACE(CstIComNode, cst_i_com_node, SYS_TYPE_OBJECT);

/* object api */
void cst_i_com_node_default_init(CstIComNodeInterface* iface) {
}

void cst_i_com_node_set_node_map (CstIComNode *self, CstNodeMap* map) {
  sys_return_if_fail(self != NULL);

  CST_I_COM_NODE_GET_IFACE(self)->set_node_map(self, map);
}


