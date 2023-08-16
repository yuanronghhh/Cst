#include <CstCore/Driver/CstRender.h>
#include <CstCore/Front/C/CstCLFlow.h>

struct _CstLFlowPrivate {
  SysChar reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstLFlow, cst_lflow, CST_TYPE_WIDGET);

CstNode* cst_lflow_new(void) {
  return sys_object_new(CST_TYPE_LFLOW, NULL);
}

static void cst_lflow_init(CstLFlow *self) {
  self->priv = cst_lflow_get_private(self);
}

static void cst_lflow_relayout_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRDraw *draw, SysInt state) {
  CST_NODE_CLASS(cst_lflow_parent_class)->relayout(v_module, v_parent, v_node, draw, state);
}

static void cst_lflow_repaint_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRDraw *draw, SysInt state) {
  FRContext *cr = fr_draw_get_cr(draw);

  CST_NODE_CLASS(cst_lflow_parent_class)->repaint(v_module, v_parent, v_node, draw, state);

  cst_node_stroke_rectangle(v_node, draw);
}

static void cst_lflow_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lflow_parent_class)->dispose(o);
}

static void cst_lflow_class_init(CstLFlowClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstNodeClass *ncls = CST_NODE_CLASS(cls);

  ocls->dispose = cst_lflow_dispose;

  ncls->relayout = cst_lflow_relayout_i;
  ncls->repaint = cst_lflow_repaint_i;
}

