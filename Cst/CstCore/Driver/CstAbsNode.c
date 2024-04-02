#include <CstCore/Driver/CstAbsNode.h>
#include <CstCore/Driver/CstILayerNode.h>

#define PNODE_TO(o, TypeName) ((TypeName *)(((SysUInt8 *)o) - offsetof(CstAbsNode, pnode)))

static void i_layer_node_imp(CstILayerNodeInterface *iface);
SYS_DEFINE_WITH_CODE(CstAbsNode, cst_abs_node, CST_TYPE_LAYER_NODE,
    SYS_IMPLEMENT_INTERFACE(CST_TYPE_I_LAYER_NODE, i_layer_node_imp));

static CstLayerNode *i_layer_node_get_children_i(CstLayerNode *o) {
  CstAbsNode *anode = CST_ABS_NODE(o);
  SysPNode *pnode = sys_pnode_next(&anode->pnode);

  return PNODE_TO(pnode, CstLayerNode);
}

static CstLayerNode* i_layer_node_get_parent_i(CstLayerNode *o) {
  CstAbsNode *anode = CST_ABS_NODE(o);
  SysPNode* pnode = sys_pnode_prev(&anode->pnode);

  return PNODE_TO(pnode, CstLayerNode);
}

static void i_layer_node_iterate_node_i(CstLayerNode* o,
  CstLayerNodeFunc func,
  SysPointer user_data) {

  CstLayerNode *node;
  CstILayerNodeInterface *iface = CST_I_LAYER_NODE_GET_IFACE(o);

  for(node = o; node; node = iface->get_next(node)) {
    if(!func(node, user_data)) {
      break;
    }
  }
}

static CstLayerNode* i_layer_node_get_next_i(CstLayerNode *o) {
  CstAbsNode *self = CST_ABS_NODE(o);
  SysPNode *pnode = sys_pnode_prev(&self->pnode);

  return PNODE_TO(pnode, CstLayerNode);
}


static void i_layer_node_imp(CstILayerNodeInterface *iface) {
  iface->get_parent = i_layer_node_get_parent_i;
  iface->get_children = i_layer_node_get_children_i;
  iface->get_next = i_layer_node_get_next_i;
  iface->iterate_node = i_layer_node_iterate_node_i;
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

  ocls->dispose = cst_abs_node_dispose;
}

void cst_abs_node_init(CstAbsNode* self) {
}

