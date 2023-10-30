#include <CstCore/Driver/CstNodeRealizer.h>


SYS_DEFINE_TYPE(CstNodeRealizerRealizer, cst_node_realizer, SYS_TYPE_OBJECT);


CstLayerNode *cst_node_realizer_realize(CstNodeRealizer* realizer, CstNode *node) {

  return cst_node_realize(layer, self->v_parent, node);
}

void cst_node_realizer_set_v_module(CstNodeRealizer *self, CstModule * v_module) {
  sys_return_if_fail(self != NULL);

  self->v_module = v_module;
}

CstModule * cst_node_realizer_get_v_module(CstNodeRealizer *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_module;
}

void cst_node_realizer_set_v_parent(CstNodeRealizer *self, CstLayerNode * v_parent) {
  sys_return_if_fail(self != NULL);

  self->v_parent = v_parent;
}

CstLayerNode * cst_node_realizer_get_v_parent(CstNodeRealizer *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_parent;
}

/* sys object api */
CstNodeRealizer* cst_node_realizer_new(void) {
  return sys_object_new(CST_TYPE_NODE_REALIZER, NULL);
}

CstNodeRealizer* cst_node_realizer_new_I(CstLayerNode *parent, CstModule *v_module, CstComNode *com_node) {
  CstNodeRealizer* o = cst_node_realizer_new();

  o->parent = parent;
  o->v_module = v_module;
  o->com_node = com_node;

  return o;
}

static void cst_node_realizer_class_init(CstNodeRealizerClass *cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_node_realizer_dispose;
}

static void cst_node_realizer_dispose(SysObject* o) {
  CstNodeRealizer *self = CST_NODE_REALIZER(o);

  SYS_OBJECT_CLASS(cst_node_realizer_parent_class)->dispose(o);
}

static void cst_node_realizer_init(CstNodeRealizer *self) {
}

