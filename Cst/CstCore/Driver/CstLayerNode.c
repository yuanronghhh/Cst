#include <CstCore/Driver/CstLayerNode.h>

#include <CstCore/Driver/CstRenderNode.h>


SYS_DEFINE_TYPE(CstLayerNode, cst_layer_node, SYS_TYPE_OBJECT);


void cst_layer_node_set_layout_node(CstLayerNode *self, CstLayoutNode *layout_node) {
  sys_return_if_fail(self != NULL);

  self->layout_node = layout_node;
}

CstLayoutNode * cst_layer_node_get_layout_node(CstLayerNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->layout_node;
}

/* object api */
static void cst_layer_node_dispose(SysObject* o) {
  CstLayerNode *self = CST_LAYER_NODE(o);

  if (self->layout_node) {

    sys_clear_pointer(&self->layout_node, _sys_object_unref);
  }
  sys_clear_pointer(&self->node, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_layer_node_parent_class)->dispose(o);
}

static void cst_layer_node_construct(CstLayerNode* self, CstNode *node) {
  self->node = node;
  sys_object_ref(node);
}

CstLayerNode *cst_layer_node_new(void) {
  return sys_object_new(CST_TYPE_LAYER_NODE, NULL);
}

CstLayerNode *cst_layer_node_new_I(CstNode *node) {
  CstLayerNode *o = cst_layer_node_new();

  cst_layer_node_construct(o, node);

  return o;
}

static void cst_layer_node_class_init(CstLayerNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layer_node_dispose;
  cls->construct = cst_layer_node_construct;
}

static void cst_layer_node_init(CstLayerNode *self) {
}
