#include <CstCore/Driver/CstILayer.h>
#include <CstCore/Driver/CstLayer.h>


SYS_DEFINE_INTERFACE(CstILayer, cst_i_layer, SYS_TYPE_OBJECT);

/* object api */
void cst_i_layer_default_init(CstILayerInterface* iface) {
}

CstLayerNode* cst_i_layer_get_root (CstLayer* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return CST_I_LAYER_GET_IFACE(self)->get_root(self);
}

void cst_i_layer_check (CstLayer *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CST_I_LAYER_GET_IFACE(self)->check(self, layout);
}

void cst_i_layer_set_root (CstLayer *self, CstLayerNode *root) {
  sys_return_if_fail(self != NULL);

  CST_I_LAYER_GET_IFACE(self)->set_root(self, root);
}

void cst_i_layer_append_node (CstLayer* self, CstLayerNode *parent, CstLayerNode* node) {
  sys_return_if_fail(self != NULL);

  CST_I_LAYER_GET_IFACE(self)->append_node(self, parent, node);
}

void cst_i_layer_new_node (CstLayer* self) {
  sys_return_if_fail(self != NULL);

  CST_I_LAYER_GET_IFACE(self)->new_node(self);
}

void cst_i_layer_iterate_node (CstLayer* self,
  CstLayerNode *lnode,
  CstLayerNodeFunc func,
  SysPointer user_data) {
  sys_return_if_fail(CST_LAYER(self));
  sys_return_if_fail(lnode != NULL);
  sys_return_if_fail(func != NULL);

  CST_I_LAYER_GET_IFACE(self)->iterate_node(self, lnode, func, user_data);
}
