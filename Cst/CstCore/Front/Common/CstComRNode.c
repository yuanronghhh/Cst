#include <CstCore/Front/Common/CstComRNode.h>

#include <CstCore/Front/Common/CstIComNode.h>
#include <CstCore/Front/Common/CstLBoxContext.h>


SYS_DEFINE_TYPE(CstComRNode, cst_com_r_node, CST_TYPE_RENDER_NODE);


CstRenderNode* cst_com_r_node_new(void) {
  return sys_object_new(CST_TYPE_COM_R_NODE, NULL);
}

/* sys object api */
static void cst_com_r_node_class_init(CstComRNodeClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_com_r_node_dispose;
}

static void cst_com_r_node_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(cst_com_r_node_parent_class)->dispose(o);
}

static void cst_com_r_node_init(CstComRNode *self) {
  CstRenderNode *rnode = CST_RENDER_NODE(self);
  CstRenderContext *rctx = cst_lbox_context_new_I();

  cst_render_node_set_rctx(rnode, rctx);
}
