#include <CstCore/Driver/CstLayerNode.h>

#include <CstCore/Driver/CstRenderNode.h>


SYS_DEFINE_TYPE(CstLayerNode, cst_layer_node, SYS_TYPE_OBJECT);


void cst_layer_node_set_rnode(CstLayerNode *self, CstRenderNode * rnode) {
  sys_return_if_fail(self != NULL);

  self->rnode = rnode;
}

CstRenderNode * cst_layer_node_get_rnode(CstLayerNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->rnode;
}

/* object api */
static void cst_layer_node_dispose(SysObject* o) {
  CstLayerNode *self = CST_LAYER_NODE(o);

  SYS_OBJECT_CLASS(cst_layer_node_parent_class)->dispose(o);
}

static void cst_layer_node_construct(CstLayerNode* self, CstRenderNode *rnode) {
  self->rnode = rnode;
}

CstLayerNode *cst_layer_node_new(void) {
  return sys_object_new(CST_TYPE_LAYER_NODE, NULL);
}

static void cst_layer_node_class_init(CstLayerNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layer_node_dispose;
  cls->construct = cst_layer_node_construct;
}

static void cst_layer_node_init(CstLayerNode *self) {
}
