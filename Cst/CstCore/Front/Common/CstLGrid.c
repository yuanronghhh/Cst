#include <CstCore/Front/Common/CstLGrid.h>


SYS_DEFINE_TYPE(CstLGrid, cst_lgrid, CST_TYPE_NODE);


CstNode* cst_lgrid_new(void) {
  return sys_object_new(CST_TYPE_LGRID, NULL);
}

CstNode* cst_lgrid_new_I(void) {
  CstNode* o = cst_lgrid_new();

  return o;
}

static void cst_lgrid_init(CstLGrid *self) {
}

static void cst_lgrid_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lgrid_parent_class)->dispose(o);
}

static void cst_lgrid_class_init(CstLGridClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstNodeClass *ncls = CST_NODE_CLASS(cls);

  ocls->dispose = cst_lgrid_dispose;
}
