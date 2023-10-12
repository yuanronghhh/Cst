#include <CstCore/Front/Common/CstLBody.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstLayout.h>


SYS_DEFINE_TYPE(CstLBody, cst_lbody, CST_TYPE_NODE);

CstNode* cst_lbody_new(void) {
  return sys_object_new(CST_TYPE_LBODY, NULL);
}

void cst_lbody_construct(CstNodeProvider *provider, CstNodeProps *v_props) {
  sys_return_if_fail(v_props != NULL);

  v_props->v_id = "id.body.0";
  v_props->v_position = CST_LAYER_BOX;

  CST_NODE_CLASS(cst_lbody_parent_class)->construct(v_module, v_component, v_parent, v_node, v_props);
}

static void cst_lbody_init(CstLBody *self) {
  CstNode *node = CST_NODE(self);

  cst_node_set_name(node, "LBody");
}

static void cst_lbody_class_init(CstLBodyClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstNodeClass * ncls = CST_NODE_CLASS(cls);

  ocls->dispose = cst_lbody_dispose;

  ncls->construct = cst_lbody_construct;
}

static void cst_lbody_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lbody_parent_class)->dispose(o);
}
