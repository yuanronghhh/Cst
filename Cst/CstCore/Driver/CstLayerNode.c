#include <CstCore/Driver/CstLayerNode.h>

#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstNode.h>


SYS_DEFINE_TYPE(CstLayerNode, cst_layer_node, SYS_TYPE_OBJECT);


CstRenderNode * cst_layer_node_get_render_node(CstLayerNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return CST_RENDER_NODE(self);
}

void cst_layer_node_repaint_node (CstLayerNode *self, CstLayout *layout) {
}

void cst_layer_node_relayout(CstLayerNode * o, CstLayout * layout) {
  CstLayerNodeClass* cls = CST_LAYER_NODE_GET_CLASS(o);

  sys_return_if_fail(cls->relayout != NULL);

  cls->relayout(o, layout);
}

static void cst_layer_node_relayout_i(CstLayerNode * o, CstLayout * layout) {
}

/* object api */
static void cst_layer_node_dispose(SysObject* o) {
  CstLayerNode *self = CST_LAYER_NODE(o);

  SYS_OBJECT_CLASS(cst_layer_node_parent_class)->dispose(o);
}

static void cst_layer_node_construct(CstLayerNode* self, CstLayer *layer, CstNode *node) {
  // self->node = node;
  self->layer = layer;

  sys_object_ref(node);
}

CstLayerNode *cst_layer_node_new(void) {
  return sys_object_new(CST_TYPE_LAYER_NODE, NULL);
}

CstLayerNode *cst_layer_node_new_I(CstLayer *layer, CstNode *node) {
  CstLayerNode *o = cst_layer_node_new();

  cst_layer_node_construct(o, layer, node);

  return o;
}

static void cst_layer_node_class_init(CstLayerNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layer_node_dispose;
  cls->construct = cst_layer_node_construct;
  cls->relayout = cst_layer_node_relayout_i;
}

static void cst_layer_node_init(CstLayerNode *self) {
}
