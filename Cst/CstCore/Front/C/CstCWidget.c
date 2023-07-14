#include <CstCore/Front/Common/CstWidgetPrivate.h>
#include <CstCore/Front/C/CstCWidget.h>


struct _CstWidgetPrivate {
  /* property */
  CstNode *component_node;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstWidget, cst_widget, CST_TYPE_NODE);
/* api */

/* widget api */
CstNode* cst_widget_new(void) {
  return sys_object_new(CST_TYPE_WIDGET, NULL);
}

static void cst_widget_init(CstWidget *self) {
  CstWidgetPrivate* priv = self->priv = cst_widget_get_private(self);

  priv->component_node = NULL;
}

static void cst_widget_relayout_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state) {
  CST_NODE_CLASS(cst_widget_parent_class)->relayout(v_module, v_parent, v_node, cr, draw, state);
}

static void cst_widget_repaint_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state) {
  CST_NODE_CLASS(cst_widget_parent_class)->repaint(v_module, v_parent, v_node, cr, draw, state);
}

static CstNode* cst_widget_realize_i(CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render) {
  CstNode *nnode = cst_node_deep_clone(v_node);

  CST_NODE_CLASS(cst_widget_parent_class)->realize(v_module, ncomp_node, v_parent, nnode, v_render);

  return nnode;
}

static CstNode* cst_widget_dclone_i(CstNode *v_node) {
  return CST_NODE_CLASS(cst_widget_parent_class)->dclone(v_node);
}

static void cst_widget_construct(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props) {
  CST_NODE_CLASS(cst_widget_parent_class)->construct(v_module, v_component, v_parent, v_node, v_props);
}

static void cst_widget_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_widget_parent_class)->dispose(o);
}

static void cst_widget_class_init(CstWidgetClass* wcls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(wcls);
  CstNodeClass *ncls = CST_NODE_CLASS(wcls);

  ocls->dispose = cst_widget_dispose;

  ncls->dclone = cst_widget_dclone_i;
  ncls->construct = cst_widget_construct;
  ncls->realize = cst_widget_realize_i;
  ncls->relayout = cst_widget_relayout_i;
  ncls->repaint = cst_widget_repaint_i;
}

