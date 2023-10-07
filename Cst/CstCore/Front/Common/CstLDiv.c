#include <CstCore/Front/Common/CstLDiv.h>
#include <CstCore/Driver/CstRender.h>

SYS_DEFINE_TYPE(CstLDiv, cst_ldiv, CST_TYPE_NODE);


CstNode* cst_ldiv_new(void) {
  return sys_object_new(CST_TYPE_LDIV, NULL);
}

static void cst_ldiv_init(CstLDiv *self) {

  cst_node_set_name(CST_NODE(self), "LDiv");
}

static void cst_ldiv_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(cst_ldiv_parent_class)->dispose(o);
}

static void cst_ldiv_class_init(CstLDivClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_ldiv_dispose;
}
