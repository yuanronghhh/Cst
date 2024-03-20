#include <CstCore/Driver/CstLayerNode.h>

#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstILayerNode.h>

SYS_DEFINE_TYPE(CstLayerNode, cst_layer_node, SYS_TYPE_OBJECT);


CstLayer* cst_layer_node_get_layer(CstLayerNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->layer;
}

void cst_layer_node_set_render_node(CstLayerNode *self, CstRenderNode* render_node) {
  sys_return_if_fail(self != NULL);

  self->render_node = render_node;
}

CstRenderNode* cst_layer_node_get_render_node(CstLayerNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->render_node;
}

/* object api */
static void cst_layer_node_dispose(SysObject* o) {
  CstLayerNode *self = CST_LAYER_NODE(o);

  sys_clear_pointer(&self->layer, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_layer_node_parent_class)->dispose(o);
}

static void cst_layer_node_construct(CstLayerNode* self, CstLayer *layer) {
  self->layer = sys_object_ref(layer);
}

CstLayerNode *cst_layer_node_new(void) {
  return sys_object_new(CST_TYPE_LAYER_NODE, NULL);
}

CstLayerNode *cst_layer_node_new_I(CstLayer *layer) {
  CstLayerNode *o = cst_layer_node_new();

  cst_layer_node_construct(o, layer);

  return o;
}

static void cst_layer_node_class_init(CstLayerNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layer_node_dispose;
  cls->construct = cst_layer_node_construct;
}

static void cst_layer_node_init(CstLayerNode *self) {
}
