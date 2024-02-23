#include <CstCore/Driver/CstDrawNode.h>

SYS_DEFINE_TYPE(CstDrawNode, cst_drawnode, CST_TYPE_LAYOUT_NODE);


/* object api */
static void cst_drawnode_construct(CstDrawNode *self) {

}

CstDrawNode* cst_drawnode_new(void) {
  return sys_object_new(CST_TYPE_DRAWNODE, NULL);
}

CstDrawNode *cst_drawnode_new_I(void) {
  CstDrawNode *o = cst_drawnode_new();

  cst_drawnode_construct(o);

  return o;
}

static void cst_drawnode_dispose(SysObject* o) {
  CstDrawNode *self = CST_DRAWNODE(o);

  SYS_OBJECT_CLASS(cst_drawnode_parent_class)->dispose(o);
}

static void cst_drawnode_class_init(CstDrawNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_drawnode_dispose;
}

void cst_drawnode_init(CstDrawNode* self) {
}

