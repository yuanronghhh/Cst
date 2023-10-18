#include <CstCore/Front/Common/CstLBoxContext.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstRenderNode.h>


SYS_DEFINE_TYPE(CstLBoxContext, cst_lbox_context, CST_TYPE_LAYOUT_CONTEXT);


void cst_lbox_layout_self_i(CstRenderContext *self, CstRenderNode *rnode, CstLayout *layout) {
}

void cst_lbox_layout_children_i(CstRenderContext *self, CstRenderNode *rnode, CstLayout *layout) {
}

/* sys object api */
CstRenderContext* cst_lbox_context_new(void) {
  return sys_object_new(CST_TYPE_LBOX_CONTEXT, NULL);
}

CstRenderContext* cst_lbox_context_new_I(void) {
  CstRenderContext* o = cst_lbox_context_new();

  return o;
}

static void cst_lbox_context_init(CstLBoxContext *o) {
}

static void cst_lbox_context_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lbox_context_parent_class)->dispose(o);
}

static void cst_lbox_context_class_init(CstLBoxContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstRenderContextClass *ncls = CST_RENDER_CONTEXT_CLASS(cls);

  ocls->dispose = cst_lbox_context_dispose;
  ncls->layout_self = cst_lbox_layout_self_i;
  ncls->layout_children = cst_lbox_layout_children_i;
}
