#include <CstCore/Front/CstLGrid.h>

SYS_DEFINE_TYPE(CstLGrid, cst_lgrid, CST_TYPE_RENDER_NODE);


static void cst_lgrid_construct_i(CstRenderNode *o, CstRenderNodeContext *param) {

  CST_RENDER_NODE_CLASS(cst_lgrid_parent_class)->construct(o, param);
}

CstRenderNode* cst_lgrid_new(void) {
  return sys_object_new(CST_TYPE_LGRID, NULL);
}

static void cst_lgrid_init(CstLGrid *self) {
}

static void cst_lgrid_dispose(SysObject* o) {


}

static void cst_lgrid_class_init(CstLGridClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstRenderNodeClass *rcls = CST_RENDER_NODE_CLASS(cls);

  ocls->dispose = cst_lgrid_dispose;
  rcls->construct = cst_lgrid_construct_i;
}
