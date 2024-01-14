#include <CstCore/Front/Common/CstLGrid.h>

#include <CstCore/Front/Common/CstLGridContext.h>


SYS_DEFINE_TYPE(CstLGrid, cst_lgrid, CST_TYPE_RENDER_NODE);


static void cst_lgrid_construct_i(CstRenderNode *o, CstNode* node) {

  CST_RENDER_NODE_CLASS(cst_lgrid_parent_class)->construct(o, node);
}

CstRenderNode* cst_lgrid_new(void) {
  return sys_object_new(CST_TYPE_LGRID, NULL);
}

CstRenderNode* cst_lgrid_new_I(CstNode *node) {
  CstRenderNode* o = cst_lgrid_new();

  cst_lgrid_construct_i(o, node);

  return o;
}

static void cst_lgrid_init(CstLGrid *self) {
  CstRenderContext *rctx = cst_lgrid_context_new_I();
  CstRenderNode *rnode = CST_RENDER_NODE(self);

  cst_render_node_set_rctx(rnode, rctx);
}

static void cst_lgrid_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(cst_lgrid_parent_class)->dispose(o);
}

static void cst_lgrid_class_init(CstLGridClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstRenderNodeClass *rcls = CST_RENDER_NODE_CLASS(cls);

  ocls->dispose = cst_lgrid_dispose;
  rcls->construct = cst_lgrid_construct_i;
}
