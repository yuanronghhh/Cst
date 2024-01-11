#include <CstCore/Driver/CstLayerNode.h>

#include <CstCore/Driver/CstRenderNode.h>


SYS_DEFINE_TYPE(CstLayerNode, cst_layer_node, SYS_TYPE_OBJECT);


void cst_layer_node_set_render_node(CstLayerNode *self, CstRenderNode * render_node) {
  sys_return_if_fail(self != NULL);

  self->render_node = render_node;
}

CstRenderNode * cst_layer_node_get_render_node(CstLayerNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->render_node;
}

void cst_layer_node_relayout_node (CstLayerNode *self, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

  CstLayerNodeClass* cls = CST_LAYER_NODE_GET_CLASS(self);
  sys_return_if_fail(cls->relayout_node != NULL);

  cls->relayout_node(self, layout);
}

void cst_layer_node_repaint_node (CstLayerNode *self, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

  CstLayerNodeClass* cls = CST_LAYER_NODE_GET_CLASS(self);
  sys_return_if_fail(cls->repaint_node != NULL);

  cls->repaint_node(self, layout);
}

const FRRect *cst_layer_node_get_bound(CstLayerNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return cst_layout_node_get_bound(self->layout_node);
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

static void cst_layer_node_construct(CstLayerNode* self, CstLayer *layer, CstNode *node) {
  self->node = node;
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
}

static void cst_layer_node_init(CstLayerNode *self) {
}
