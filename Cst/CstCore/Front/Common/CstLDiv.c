#include <CstCore/Front/Common/CstLDiv.h>
#include <CstCore/Driver/CstRender.h>

SYS_DEFINE_TYPE(CstLDiv, cst_ldiv, CST_TYPE_NODE);

CstNode* cst_ldiv_new(void) {
  return sys_object_new(CST_TYPE_LDIV, NULL);
}

static void cst_ldiv_init(CstLDiv *self) {

  cst_render_node_set_name(CST_NODE(self), "LDiv");
}

static void cst_ldiv_relayout_i(CstNode *self, CstLayout *layout) {

  CST_NODE_CLASS(cst_ldiv_parent_class)->relayout(self, layout);
}

static void cst_ldiv_repaint_i(CstNode *self, CstLayout *layout) {
  CST_NODE_CLASS(cst_ldiv_parent_class)->repaint(self, layout);

  cst_render_node_stroke_rectangle(self, layout);
}

static void cst_ldiv_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_ldiv_parent_class)->dispose(o);
}

static void cst_ldiv_class_init(CstLDivClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstNodeClass *ncls = CST_NODE_CLASS(cls);

  ocls->dispose = cst_ldiv_dispose;

  ncls->relayout = cst_ldiv_relayout_i;
  ncls->repaint = cst_ldiv_repaint_i;
}
