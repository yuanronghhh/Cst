#include <CstCore/Front/Common/CstLBox.h>
#include <CstCore/Front/Common/CstLBoxContext.h>


SYS_DEFINE_TYPE(CstLBox, cst_lbox, CST_TYPE_RENDER_NODE);

static void cst_lbox_construct_i(CstRenderNode *o, CstNode* node) {

  CST_RENDER_NODE_CLASS(cst_lbox_parent_class)->construct(o, node);
}

CstRenderNode* cst_lbox_new(void) {
  return sys_object_new(CST_TYPE_LBOX, NULL);
}

CstRenderNode* cst_lbox_new_I(CstNode *node) {
  CstRenderNode* o = cst_lbox_new();

  cst_lbox_construct_i(o, node);

  return o;
}

static void cst_lbox_init(CstLBox *self) {
  CstRenderContext *rctx = cst_lbox_context_new_I();
  CstRenderNode *rnode = CST_RENDER_NODE(self);

  cst_render_node_set_render_ctx(rnode, rctx);
}

static void cst_lbox_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lbox_parent_class)->dispose(o);
}

static void cst_lbox_class_init(CstLBoxClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstRenderNodeClass *rcls = CST_RENDER_NODE_CLASS(cls);

  ocls->dispose = cst_lbox_dispose;
  rcls->construct = cst_lbox_construct_i;
}
