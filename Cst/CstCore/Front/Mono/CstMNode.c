#include <CstCore/Front/Mono/CstMNode.h>


SysType cst_node_get_type(void) {
  static SysType meta = 0;

  if (meta != 0) {
    return meta;
  }

  FRMonoContext *c = cst_context_get_mono_context();
  meta = fr_mono_get_meta_from_name(c, "CstGUI.CstCore", "CstNode");

  return meta;
}

CstNode* cst_node_new(void) {
  return fr_mono_object_new(CST_TYPE_NODE);
}

void _cst_node_construct(CstNodeContext *ctx, CstNodeParam *param) {
  sys_return_if_fail(param != NULL);
  sys_return_if_fail(node != NULL);
}

void _cst_node_realize(CstModule *v_module, CstNode *v_parent, CstNode *v_node) {

}

void _cst_node_set_x(CstNode *node, SysInt x) {
}

void _cst_node_set_y(CstNode *node, SysInt y) {
}
