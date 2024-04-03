#include <CstCore/Driver/CstDrawNode.h>

SYS_DEFINE_TYPE(CstDrawNode, cst_draw_node, SYS_TYPE_OBJECT);


/* object api */
static void cst_draw_node_construct(CstDrawNode *self) {

}

CstDrawNode* cst_draw_node_new(void) {
  return sys_object_new(CST_TYPE_DRAW_NODE, NULL);
}

CstDrawNode *cst_draw_node_new_I(void) {
  CstDrawNode *o = cst_draw_node_new();

  cst_draw_node_construct(o);

  return o;
}

static void cst_draw_node_dispose(SysObject* o) {
  // CstDrawNode *self = CST_DRAW_NODE(o);

  SYS_OBJECT_CLASS(cst_draw_node_parent_class)->dispose(o);
}

static void cst_draw_node_class_init(CstDrawNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_draw_node_dispose;
}

void cst_draw_node_init(CstDrawNode* self) {
}

