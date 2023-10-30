#include <CstCore/Driver/CstNodeRealizer.h>

SYS_DEFINE_TYPE(CstNodeRealizerRealizer, cst_node_realizer, SYS_TYPE_OBJECT);


void cst_node_realizer_realize(CstNodeRealizer* realizer, CstNode *node, CstLayout *layout) {
  return cst_node_realize(node, self);
}

/* sys object api */
CstNodeRealizer* cst_node_realizer_new(void) {
  return sys_object_new(CST_TYPE_NODE_REALIZER, NULL);
}

CstNodeRealizer* cst_node_realizer_new_I(CstNodeRealizer *pass, CstComNode *com_node) {
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

