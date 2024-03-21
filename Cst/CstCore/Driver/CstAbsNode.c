#include <CstCore/Driver/CstAbsNode.h>

SYS_DEFINE_TYPE(CstAbsNode, cst_abs_node, CST_TYPE_LAYER_NODE);


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
  CstAbsNode *self = CST_ABS_NODE(o);

  SYS_OBJECT_CLASS(cst_abs_node_parent_class)->dispose(o);
}

static void cst_abs_node_class_init(CstAbsNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_abs_node_dispose;
}

void cst_abs_node_init(CstAbsNode* self) {
}

