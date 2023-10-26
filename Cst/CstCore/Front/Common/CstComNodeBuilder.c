#include <CstCore/Front/Common/CstComNodeBuilder.h>

#include <CstCore/Front/Common/CstComNode.h>
#include <CstCore/Driver/CstModule.h>
#include <Framework/Event/Action/FRAWatch.h>


SYS_DEFINE_TYPE(CstComNodeBuilder, cst_com_node_builder, CST_TYPE_NODE_BUILDER);


/* parse */
static SysBool com_node_builder_parse_action_bind(CstComNodeBuilder *self, const SysChar *watch_name, const SysChar *func_name, SysChar **bind_var) {
  return true;
}

SysBool cst_com_node_builder_parse_action(CstComNodeBuilder *self, const SysChar *watch_name, const SysChar *func_name) {
  return true;
}

void cst_com_node_builder_build_com_node(CstComNodeBuilder* self, CstNode *node) {
  CstComNode* com_node = CST_COM_NODE(node);

  com_node_set_node_map(com_node, self->value);
}

/* object api */
static void cst_com_node_builder_dispose(SysObject* o) {
  CstComNodeBuilder *self = CST_COM_NODE_BUILDER(o);

  SYS_OBJECT_CLASS(cst_com_node_builder_parent_class)->dispose(o);
}

CstNodeBuilder *cst_com_node_builder_new(void) {
  return sys_object_new(CST_TYPE_NODE_BUILDER, NULL);
}

void cst_com_node_builder_construct_i(CstNodeBuilder *o, CstModule* v_module, CstComponent* v_component, CstNode* v_pnode) {

  CST_NODE_BUILDER_CLASS(cst_com_node_builder_parent_class)->construct(o, v_module, v_component, v_pnode);
}

CstNodeBuilder* cst_com_node_builder_new_I(CstModule* v_module, CstComponent* v_component, CstNode* v_pnode) {
  CstNodeBuilder* o = cst_com_node_builder_new();

  cst_com_node_builder_construct_i(o, v_module, v_component, v_pnode);

  return o;
}

static void cst_com_node_builder_class_init(CstComNodeBuilderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstNodeBuilderClass *ncls = CST_NODE_BUILDER_CLASS(cls);

  ocls->dispose = cst_com_node_builder_dispose;
  ncls->construct = cst_com_node_builder_construct_i;
}

static void cst_com_node_builder_init(CstComNodeBuilder *self) {
}

