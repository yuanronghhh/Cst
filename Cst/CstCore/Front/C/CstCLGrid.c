#include <CstCore/Front/Common/CstLGridPrivate.h>
#include <CstCore/Front/C/CstCLGrid.h>
#include <CstCore/Driver/Css/CstCss.h>


struct _CstLGridPrivate {
  SysChar reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstLGrid, cst_lgrid, CST_TYPE_WIDGET);

CstNode* _cst_lgrid_new(void) {
  return sys_object_new(CST_TYPE_LGRID, NULL);
}

static void cst_lgrid_init(CstLGrid *self) {
  self->priv = cst_lgrid_get_private(self);

  CstNode *node = CST_NODE(self);
  CstCssClosure *c;

  c = cst_css_closure_new(NULL, cst_constrain_same_width, NULL);
  cst_node_set_child_width_closure(node, c);

  c = cst_css_closure_new(NULL, cst_constrain_same_height, NULL);
  cst_node_set_child_height_closure(node, c);

  cst_node_set_name(node, "LGrid");
}

static void cst_lgrid_relayout_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state) {
  SysInt w, h;

  cst_node_get_prefer_size(v_node, &w, &h);
  cst_node_set_size(v_node, w, h);
  cst_node_relayout_h(v_module, v_parent, v_node, cr, draw, state);

  CST_NODE_CLASS(cst_lgrid_parent_class)->relayout(v_module, v_parent, v_node, cr, draw, state);
}

static void cst_lgrid_repaint_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state) {
  CST_NODE_CLASS(cst_lgrid_parent_class)->repaint(v_module, v_parent, v_node, cr, draw, state);

  cst_node_stroke_rectangle(v_node, cr);
}

static void cst_lgrid_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lgrid_parent_class)->dispose(o);
}

static void cst_lgrid_construct(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props) {
  CST_NODE_CLASS(cst_lgrid_parent_class)->construct(v_module, v_component, v_parent, v_node, v_props);
}

static void cst_lgrid_class_init(CstLGridClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstNodeClass *ncls = CST_NODE_CLASS(cls);

  ocls->dispose = cst_lgrid_dispose;

  ncls->construct = cst_lgrid_construct;
  ncls->relayout = cst_lgrid_relayout_i;
  ncls->repaint = cst_lgrid_repaint_i;
}
