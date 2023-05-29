#include <CstCore/Front/Common/CstLBoxPrivate.h>
#include <CstCore/Front/C/CstCLBox.h>
#include <CstCore/Driver/CstRender.h>

struct _CstLBoxPrivate {
  SysInt reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstLBox, cst_lbox, CST_TYPE_WIDGET);

CstNode* cst_lbox_new(void) {
  return sys_object_new(CST_TYPE_LBOX, NULL);
}

static void cst_lbox_init(CstLBox *self) {
  self->priv = cst_lbox_get_private(self);

  cst_node_set_name(CST_NODE(self), "LBox");
}

static CstNode* cst_lbox_realize_i(CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render) {

  return CST_NODE_CLASS(cst_lbox_parent_class)->realize(v_module, ncomp_node, v_parent, v_node, v_render);
}

static void cst_lbox_relayout_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
 
  cst_node_relayout_h(v_module, v_parent, v_node, cr);

  CST_NODE_CLASS(cst_lbox_parent_class)->relayout(v_module, v_parent, v_node, cr);
}

static void cst_lbox_relayout_down_i(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  CST_NODE_CLASS(cst_lbox_parent_class)->relayout_down(v_module, v_component, v_parent, v_node, cr);
}

static void cst_lbox_repaint_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  CST_NODE_CLASS(cst_lbox_parent_class)->repaint(v_module, v_parent, v_node, cr);

  cst_node_stroke_rectangle(v_node, cr);
}

static void cst_lbox_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lbox_parent_class)->dispose(o);
}

static void cst_lbox_class_init(CstLBoxClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstNodeClass *ncls = CST_NODE_CLASS(cls);

  ocls->dispose = cst_lbox_dispose;

  ncls->realize = cst_lbox_realize_i;
  ncls->relayout = cst_lbox_relayout_i;
  ncls->relayout_down = cst_lbox_relayout_down_i;
  ncls->repaint = cst_lbox_repaint_i;
}
