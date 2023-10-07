#include <CstCore/Driver/CstRender.h>
#include <CstCore/Front/Common/CstLFlow.h>

struct _CstLFlowPrivate {
  SysChar reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstLFlow, cst_lflow, CST_TYPE_LAYOUT_CONTEXT);

CstNode* cst_lflow_new(void) {
  return sys_object_new(CST_TYPE_LFLOW, NULL);
}

static void cst_lflow_init(CstLFlow *self) {
  self->priv = cst_lflow_get_private(self);
}

static void cst_lflow_relayout_i(CstRenderNode *self, CstLayout *layout) {
  CST_NODE_CLASS(cst_lflow_parent_class)->relayout(self, layout);
}

static void cst_lflow_repaint_i(CstRenderNode *self, CstLayout *layout) {
  CST_NODE_CLASS(cst_lflow_parent_class)->repaint(self, layout);

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

