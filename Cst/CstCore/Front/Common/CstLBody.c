#include <CstCore/Front/Common/CstLBody.h>

#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Front/Common/CstLBodyContext.h>


SYS_DEFINE_TYPE(CstLBody, cst_lbody, CST_TYPE_RENDER_NODE);


static SysObject* cst_lbody_dclone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  SysObject *n = SYS_OBJECT_CLASS(cst_lbody_parent_class)->dclone(o);

  return n;
}

CstRenderNode* cst_lbody_new(void) {
  return sys_object_new(CST_TYPE_LBODY, NULL);
}

void cst_lbody_construct(CstRenderNode *o, CstNode *node) {
  sys_return_if_fail(o != NULL);
  CST_RENDER_NODE_CLASS(cst_lbody_parent_class)->construct(o, node);

  cst_render_node_set_id(o, "id.body.0");
}

static void cst_lbody_init(CstLBody *self) {
  CstRenderContext *rctx = cst_lbody_context_new_I();
  CstRenderNode *rnode = CST_RENDER_NODE(self);

  cst_render_node_set_rctx(rnode, rctx);
}

static void cst_lbody_class_init(CstLBodyClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstRenderNodeClass *ncls = CST_RENDER_NODE_CLASS(cls);

  ocls->dispose = cst_lbody_dispose;
  ocls->dclone = cst_lbody_dclone_i;
  ncls->construct = cst_lbody_construct;
}

static void cst_lbody_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lbody_parent_class)->dispose(o);
}
