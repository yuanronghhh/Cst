#include <CstCore/Front/C/CstCLFlex.h>

struct _CstLFlexPrivate {
  SysInt reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstLFlex, cst_lflex, CST_TYPE_WIDGET);

CstNode* cst_lflex_new(void) {
  return sys_object_new(CST_TYPE_LFLEX, NULL);
}

static void cst_lflex_init(CstLFlex *self) {
  self->priv = cst_lflex_get_private(self);
}

static void cst_lflex_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lflex_parent_class)->dispose(o);
}

static void cst_lflex_class_init(CstLFlexClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_lflex_dispose;
}

