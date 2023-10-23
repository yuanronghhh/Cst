#include <CstCore/Front/Common/CstLGrid.h>
#include <CstCore/Front/Common/CstLGridContext.h>


SYS_DEFINE_TYPE(CstLGrid, cst_lgrid, CST_TYPE_NODE);


void cst_lgrid_layout_i (CstLayoutNode* o, CstLayout *layout) {
}

CstNode* cst_lgrid_new(void) {
  return sys_object_new(CST_TYPE_LGRID, NULL);
}

CstNode* cst_lgrid_new_I(void) {
  CstNode* o = cst_lgrid_new();

  return o;
}

static void cst_lgrid_init(CstLGrid *self) {
  CstNode *node = CST_NODE(self);

  cst_node_set_name(node, "LGrid");
  cst_node_set_rctx_type(node, CST_TYPE_LGRID_CONTEXT);
}

static void cst_lgrid_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lgrid_parent_class)->dispose(o);
}

static void cst_lgrid_class_init(CstLGridClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstLayoutNodeClass *lcls = CST_LAYOUT_NODE_CLASS(cls);

  ocls->dispose = cst_lgrid_dispose;
  lcls->layout = cst_lgrid_layout_i;
}
