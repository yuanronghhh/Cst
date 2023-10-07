#include <CstCore/Front/Common/CstLBox.h>


SYS_DEFINE_TYPE(CstLBox, cst_lbox, CST_TYPE_NODE);


CstNode* cst_lbox_new(void) {
  return sys_object_new(CST_TYPE_LBOX, NULL);
}

CstNode* cst_lbox_new_I(void) {
  CstNode* o = cst_lbox_new();

  return o;
}

static void cst_lbox_init(CstLBox *self) {
}

static void cst_lbox_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lbox_parent_class)->dispose(o);
}

static void cst_lbox_class_init(CstLBoxClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstNodeClass *ncls = CST_NODE_CLASS(cls);

  ocls->dispose = cst_lbox_dispose;
}
