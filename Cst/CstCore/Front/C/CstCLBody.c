#include <CstCore/Front/Common/CstLBodyPrivate.h>
#include <CstCore/Front/C/CstCLBody.h>
#include <CstCore/Driver/CstRender.h>


struct _CstLBodyPrivate {
  SysInt reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstLBody, cst_lbody, CST_TYPE_NODE);

CstNode* _cst_lbody_new(void) {
  return sys_object_new(CST_TYPE_LBODY, NULL);
}

void cst_lbody_construct(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props) {
  sys_return_if_fail(v_props != NULL);

  v_props->v_id = "id.body.0";

  CST_NODE_CLASS(cst_lbody_parent_class)->construct(v_module, v_component, v_parent, v_node, v_props);
}

static void cst_lbody_init(CstLBody *self) {
  self->priv = cst_lbody_get_private(self);
  CstNode *node = CST_NODE(self);

  self->priv = cst_lbody_get_private(self);

  cst_node_set_name(node, "LBody");
}

static void cst_lbody_relayout_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
}

static void cst_lbody_relayout_down_i(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  if (v_node->children) {
    CST_NODE_CLASS(cst_lbody_parent_class)->relayout_down(v_module, v_component, v_node, v_node->children, cr);
  }
}

static void cst_lbody_repaint_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  const FRRect *rect;

  CST_NODE_CLASS(cst_lbody_parent_class)->repaint(v_module, v_parent, v_node, cr);

  rect = cst_node_get_bound(v_node);

  fr_context_rectangle(cr, rect->x, rect->y, rect->width, rect->height);
  fr_context_fill(cr);
}

static CstNode* cst_lbody_realize_i(CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render) {
  sys_return_val_if_fail(v_node != NULL, NULL);

  CST_NODE_CLASS(cst_lbody_parent_class)->realize(v_module, ncomp_node, v_parent, v_node, v_render);

  SysInt width = 0, height = 0;

  cst_render_get_size(v_render, &width, &height);
  cst_node_set_size(v_node, width, height);

  return v_node;
}

static void cst_lbody_class_init(CstLBodyClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstNodeClass * ncls = CST_NODE_CLASS(cls);

  ocls->dispose = cst_lbody_dispose;

  ncls->construct = cst_lbody_construct;
  ncls->repaint = cst_lbody_repaint_i;
  ncls->relayout = cst_lbody_relayout_i;
  ncls->relayout_down = cst_lbody_relayout_down_i;
  ncls->realize = cst_lbody_realize_i;
}

static void cst_lbody_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lbody_parent_class)->dispose(o);
}
