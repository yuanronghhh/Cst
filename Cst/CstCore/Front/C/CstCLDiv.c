#include <CstCore/Front/Common/CstLDiv.h>
#include <CstCore/Front/C/CstCLDiv.h>
#include <CstCore/Driver/CstRender.h>

struct _CstLDivPrivate {
  SysInt reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstLDiv, cst_ldiv, CST_TYPE_WIDGET);

CstNode* cst_ldiv_new(void) {
  return sys_object_new(CST_TYPE_LDIV, NULL);
}

static void cst_ldiv_init(CstLDiv *self) {
  self->priv = cst_ldiv_get_private(self);

  cst_node_set_name(CST_NODE(self), "LDiv");
}

static void cst_ldiv_relayout_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  cst_node_relayout_v(v_module, v_parent, v_node, cr);

  CST_NODE_CLASS(cst_ldiv_parent_class)->relayout(v_module, v_parent, v_node, cr);
}

static void cst_ldiv_relayout_down_i(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  CST_NODE_CLASS(cst_ldiv_parent_class)->relayout_down(v_module, v_component, v_parent, v_node, cr);
}

static void cst_ldiv_repaint_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  CST_NODE_CLASS(cst_ldiv_parent_class)->repaint(v_module, v_parent, v_node, cr);

  cst_node_stroke_rectangle(v_node, cr);
}

static void cst_ldiv_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_ldiv_parent_class)->dispose(o);
}

static void cst_ldiv_class_init(CstLDivClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstNodeClass *ncls = CST_NODE_CLASS(cls);

  ocls->dispose = cst_ldiv_dispose;

  ncls->relayout = cst_ldiv_relayout_i;
  ncls->relayout_down = cst_ldiv_relayout_down_i;
  ncls->repaint = cst_ldiv_repaint_i;
}
