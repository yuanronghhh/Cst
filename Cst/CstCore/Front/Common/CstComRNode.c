#include <CstCore/Front/Common/CstComRNode.h>

#include <CstCore/Front/Common/CstLBoxContext.h>

SYS_DEFINE_TYPE(CstComRNode, cst_com_rnode, CST_TYPE_RENDER_NODE);


CstRenderNode* cst_com_rnode_new(void) {
  return sys_object_new(CST_TYPE_COM_RNODE, NULL);
}

/* sys object api */
static void cst_com_rnode_class_init(CstComRNodeClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_com_rnode_dispose;
}

static void cst_com_rnode_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(cst_com_rnode_parent_class)->dispose(o);
}

static void cst_com_rnode_init(CstComRNode *self) {
  CstRenderNode *rnode = CST_RENDER_NODE(self);
  CstRenderContext *rctx = cst_lbox_context_new_I();

  cst_render_node_set_rctx(rnode, rctx);
}
