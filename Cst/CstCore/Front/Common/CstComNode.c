#include <CstCore/Front/Common/CstComNode.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Front/CstFrontCore.h>


SYS_DEFINE_TYPE(CstComNode, cst_com_node, CST_TYPE_NODE);


CstNode* cst_com_node_new(void) {
  return sys_object_new(CST_TYPE_COM_NODE, NULL);
}

CstNode* cst_com_node_new_with_component(CstComponent *comp) {
  sys_return_val_if_fail(comp != NULL, NULL);

  SysChar *tname;
  CstNode *nnode = cst_com_node_new();
  CstComNode *self = CST_COM_NODE(nnode);

  self->component = comp;
  sys_object_ref(comp);

  tname = sys_strdup_printf("<%s>", cst_component_get_id(comp));
  cst_node_set_name(nnode, tname);
  sys_free_N(tname);

  return nnode;
}

CstComponent *cst_com_node_get_component(CstComNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->component;
}

CstPropValue * cst_com_node_get_value(CstComNode * self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);

  return sys_hash_table_lookup(self->values_ht, (SysPointer)key);
}

void com_node_set_value(CstComNode *self, const SysChar *key, CstPropValue *value) {
  sys_return_if_fail(self != NULL);

  sys_hash_table_insert(self->values_ht, (SysPointer)sys_strdup(key), (SysPointer)value);
}

static void text_set_text_i(CstRenderNode *rnode, const SysChar *key, const SysChar *bind_var, CstPropValue *value) {
  CstText *text = CST_TEXT(rnode);

  cst_text_set_text(text, sys_value_v_string(value));
}

static void node_set_awatch_func_i(CstRenderNode *rnode, const SysChar *key, const SysChar *bind_var, CstPropValue *value) {
  SysType tp = fr_awatch_get_type_by_name(key);
  SysList* list = cst_render_node_get_awatch_list(rnode);
  FRAWatch *awatch = fr_awatch_list_get_awatch(list, tp, bind_var);

  if (awatch == NULL) {
    sys_warning_N("Not found awatch in node: %s, %s, %s", 
      cst_render_node_get_name(rnode),
      cst_render_node_get_id(rnode),
      key);

    return;
  }

  fr_awatch_set_function(awatch, (FREventFunc)sys_value_v_pointer(value));
}

static void com_node_set_value_i(CstRenderNode *rnode, const SysChar *key, const SysChar *bind_var, CstPropValue *value) {
  CstNode* node = cst_render_node_get_node(rnode);
  CstComNode *com_node = CST_COM_NODE(node);

  com_node_set_value(com_node, key, value);
}

CstNodeMapFunc cst_com_node_get_func(SysType node_type, SysInt prop_type, SysInt data_type) {
  switch(data_type) {
    case SYS_STRING:
      if(node_type == CST_TYPE_TEXT && prop_type == CST_NODE_PROP_VALUE) {
        return text_set_text_i;
      }

      if(node_type == CST_TYPE_COM_NODE && prop_type == CST_NODE_PROP_VALUE) {
        return com_node_set_value_i;
      }
      break;
    case SYS_POINTER:
      if (sys_type_is_a(node_type, CST_TYPE_NODE) && prop_type == CST_NODE_PROP_ACTION) {
        return node_set_awatch_func_i;
      }
      break;
  }

  sys_warning_N("Not support prop map for component node: %d,%d", prop_type, data_type);
  return NULL;
}

/* sys object api */
void cst_com_node_construct_i(CstNode *v_node, CstNodeBuilder *builder) {

  CST_NODE_CLASS(cst_com_node_parent_class)->construct(v_node, builder);
}

static CstRenderNode* cst_com_node_realize_i(CstNode* o, CstRenderNode* prnode, CstLayout* layout) {
  CstComNode *self = CST_COM_NODE(o);
  CstRenderNode* rnode = CST_NODE_CLASS(cst_com_node_parent_class)->realize(o, prnode, layout);

  return cst_component_realize(self->component, rnode, layout);
}

static void cst_com_node_class_init(CstComNodeClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstNodeClass *ncls = CST_NODE_CLASS(cls);

  ncls->construct = cst_com_node_construct_i;
  ncls->realize = cst_com_node_realize_i;
  ocls->dispose = cst_com_node_dispose;
}

static void cst_com_node_dispose(SysObject* o) {
  CstComNode *self = CST_COM_NODE(o);
  sys_object_unref(self->component);
  sys_hash_table_unref(self->values_ht);

  SYS_OBJECT_CLASS(cst_com_node_parent_class)->dispose(o);
}

static void cst_com_node_init(CstComNode *self) {
  SysHashTable *ht;

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, sys_free, (SysDestroyFunc)sys_value_unref);

  self->values_ht = ht;
}
