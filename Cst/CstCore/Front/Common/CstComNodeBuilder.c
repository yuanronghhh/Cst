#include <CstCore/Front/Common/CstComNodeBuilder.h>

#include <CstCore/Front/Common/CstComNode.h>
#include <CstCore/Driver/CstModule.h>
#include <Framework/Event/Action/FRAWatch.h>


SYS_DEFINE_TYPE(CstComNodeBuilder, cst_com_node_builder, CST_TYPE_NODE_BUILDER);


void cst_com_node_builder_set_component(CstComNodeBuilder *self, CstComponent * component) {
  sys_return_if_fail(self != NULL);

  self->component = component;
}

CstComponent * cst_com_node_builder_get_component(CstComNodeBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->component;
}

CstNodeMap* cst_com_node_builder_get_node_map(CstComNodeBuilder * self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);

  return sys_hash_table_lookup(self->values_ht, (SysPointer)key);
}

void cst_com_node_builder_set_node_map(CstComNodeBuilder* self, CstNodeMap *map) {
  sys_return_if_fail(self != NULL);
  FRPair *pair = FR_PAIR(map);

  sys_hash_table_insert(self->values_ht, (SysPointer)pair->key, (SysPointer)map);
}

/* parse */
static SysBool com_node_builder_parse_action_bind(CstComNodeBuilder *self, const SysChar *watch_name, const SysChar *func_name, SysChar **bind_var) {
  return true;
}

SysBool cst_com_node_builder_parse_action(CstComNodeBuilder *self, const SysChar *watch_name, const SysChar *func_name) {
  return true;
}

void cst_com_node_builder_parse_i(CstNodeBuilder *o, JNode *jnode) {
  CST_NODE_BUILDER_CLASS(cst_com_node_builder_parent_class)->parse(o, jnode);

  ast_com_node_parse(o, jnode);
}

void cst_com_node_builder_build_i(CstNodeBuilder *o, CstNode *node) {
  CST_NODE_BUILDER_CLASS(cst_com_node_builder_parent_class)->build(o, node);

  CstComNodeBuilder *self = CST_COM_NODE_BUILDER(o);
  CstComNode *com_node = CST_COM_NODE(node);

  cst_com_node_set_component(com_node, self->component);
  cst_com_node_set_values_ht(com_node, self->values_ht);
}

/* object api */
static void cst_com_node_builder_dispose(SysObject* o) {
  CstComNodeBuilder *self = CST_COM_NODE_BUILDER(o);

  SYS_OBJECT_CLASS(cst_com_node_builder_parent_class)->dispose(o);
}

CstNodeBuilder *cst_com_node_builder_new(void) {
  return sys_object_new(CST_TYPE_COM_NODE_BUILDER, NULL);
}

void cst_com_node_builder_construct_i(CstNodeBuilder *o, CstContext *c, CstNode* v_pnode) {

  CST_NODE_BUILDER_CLASS(cst_com_node_builder_parent_class)->construct(o, v_module, v_component, v_pnode);
}

CstNodeBuilder* cst_com_node_builder_new_I(CstContext *c, CstNode* v_pnode) {
  CstNodeBuilder* o = cst_com_node_builder_new();

  cst_com_node_builder_construct_i(o, v_module, v_component, v_pnode);

  return o;
}

static void cst_com_node_builder_class_init(CstComNodeBuilderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstNodeBuilderClass *ncls = CST_NODE_BUILDER_CLASS(cls);

  ocls->dispose = cst_com_node_builder_dispose;
  ncls->construct = cst_com_node_builder_construct_i;
  ncls->parse = cst_com_node_builder_parse_i;
  ncls->build = cst_com_node_builder_build_i;
}

static void cst_com_node_builder_init(CstComNodeBuilder *self) {
  self->values_ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
}

