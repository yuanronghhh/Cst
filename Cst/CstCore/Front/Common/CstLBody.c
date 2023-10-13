#include <CstCore/Front/Common/CstLBody.h>
#include <CstCore/Front/Common/CstLBodyContext.h>
#include <CstCore/Driver/CstNodeBuilder.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstLayout.h>


SYS_DEFINE_TYPE(CstLBody, cst_lbody, CST_TYPE_NODE);


static CstRenderContext* cst_body_new_default_context_i(CstNode* node) {

  return cst_lbody_context_new_I();
}

CstNode* cst_lbody_new(void) {
  return sys_object_new(CST_TYPE_LBODY, NULL);
}

void cst_lbody_construct(CstNode *v_node, CstNodeBuilder *builder) {
  sys_return_if_fail(v_node != NULL);
  sys_return_if_fail(builder != NULL);

  cst_node_builder_set_id(builder, "id.body.0");
  cst_node_builder_set_position(builder, CST_LAYER_BOX);

  CST_NODE_CLASS(cst_lbody_parent_class)->construct(v_node, builder);
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
  ncls->new_default_context = cst_body_new_default_context_i;
}

static void cst_lbody_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lbody_parent_class)->dispose(o);
}
