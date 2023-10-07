#include <CstCore/Front/Common/CstLBody.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstLayout.h>


SYS_DEFINE_TYPE(CstLBody, cst_lbody, CST_TYPE_NODE);

CstNode* cst_lbody_new(void) {
  return sys_object_new(CST_TYPE_LBODY, NULL);
}

void cst_lbody_construct(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props) {
  sys_return_if_fail(v_props != NULL);

  v_props->v_id = "id.body.0";

  CST_NODE_CLASS(cst_lbody_parent_class)->construct(v_module, v_component, v_parent, v_node, v_props);
}

static void cst_lbody_init(CstLBody *self) {
  CstNode *node = CST_NODE(self);

  cst_render_node_set_name(node, "LBody");
}

static void cst_lbody_relayout_i(CstNode *self, CstLayout *layout) {
  SysInt width = 0, height = 0;
  FRDraw* draw = cst_layout_get_draw(layout);

  fr_draw_get_size(draw, &width, &height);
  cst_render_node_set_size(v_node, width, height);
}

static void cst_lbody_repaint_i(CstNode *self, CstLayout *layout) {
  CST_NODE_CLASS(cst_lbody_parent_class)->repaint(self, layout);

  cst_render_node_fill_rectangle(v_node, draw);
}

static CstNode* cst_lbody_realize_i(CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render) {
  sys_return_val_if_fail(v_node != NULL, NULL);

  CST_NODE_CLASS(cst_lbody_parent_class)->realize(v_module, ncomp_node, v_parent, v_node, v_render);

  return v_node;
}

static void cst_lbody_class_init(CstLBodyClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstNodeClass * ncls = CST_NODE_CLASS(cls);

  ocls->dispose = cst_lbody_dispose;

  ncls->construct = cst_lbody_construct;
  ncls->repaint = cst_lbody_repaint_i;
  ncls->relayout = cst_lbody_relayout_i;
  ncls->realize = cst_lbody_realize_i;
}

static void cst_lbody_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lbody_parent_class)->dispose(o);
}
