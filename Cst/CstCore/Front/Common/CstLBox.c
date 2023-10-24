#include <CstCore/Front/Common/CstLBox.h>
#include <CstCore/Front/Common/CstLBoxContext.h>

SYS_DEFINE_TYPE(CstLBox, cst_lbox, CST_TYPE_RENDER_NODE);


CstRenderNode* cst_lbox_new(void) {
  return sys_object_new(CST_TYPE_LBOX, NULL);
}

CstRenderNode* cst_lbox_new_I(void) {
  CstRenderNode* o = cst_lbox_new();

  return o;
}

static void cst_lbox_init(CstLBox *self) {
}

static void cst_lbox_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lbox_parent_class)->dispose(o);
}

static void cst_lbox_class_init(CstLBoxClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_lbox_dispose;
}
