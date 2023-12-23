#include <CstCore/Front/Common/CstComNode.h>

#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstNodeBuilder.h>
#include <CstCore/Front/CstFrontCore.h>


SYS_DEFINE_TYPE(CstComNode, cst_com_node, CST_TYPE_NODE);


void cst_com_node_set_component(CstComNode *self, CstComponent * component) {
  sys_return_if_fail(self != NULL);

  self->component = component;
}

CstComponent * cst_com_node_get_component(CstComNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  sys_assert(self->component == NULL);

  return self->component;
}

void cst_com_node_set_values_ht(CstComNode *self, SysHashTable * values_ht) {
  sys_return_if_fail(self != NULL);

  sys_assert(self->values_ht == NULL);

  self->values_ht = values_ht;
}

SysHashTable * cst_com_node_get_values_ht(CstComNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->values_ht;
}

CstNodeMap* cst_com_node_get_node_map(CstComNode * self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);

  return sys_hash_table_lookup(self->values_ht, (SysPointer)key);
}

void cst_com_node_set_node_map(CstComNode* self, CstNodeMap *map) {
  sys_return_if_fail(self != NULL);
  FRPair *pair = FR_PAIR(map);

  sys_hash_table_insert(self->values_ht, (SysPointer)pair->key, (SysPointer)map);
}

static void text_set_text_i(CstRenderNode *rnode, CstNodeMap *map) {
  CstText *text = CST_TEXT(rnode);
  FRPair *pair = FR_PAIR(map);

  cst_text_set_text(text, sys_value_get_v_string(pair->value));
}

static void node_set_awatch_func_i(CstRenderNode* rnode, CstNodeMap* map) {
  FRPair *pair = FR_PAIR(map);
  SysList *awatch_list = cst_render_node_get_awatch_list(rnode);
  FRAWatch *mawatch = FR_AWATCH(pair->value);

  SysType tp = sys_type_from_instance(mawatch);
  FRAWatch* awatch = fr_awatch_list_get_awatch(awatch_list, tp, sys_value_get_v_string(pair->value));

  if (awatch == NULL) {

    sys_warning_N("Not found awatch in node: %s, %s, %s",
      cst_render_node_get_name(rnode),
      cst_render_node_get_id(rnode),
      pair->key);
    return;
  }

  fr_awatch_set_func(awatch, (FREventFunc)fr_awatch_get_func(mawatch));
}

static void com_node_set_value_i(CstRenderNode* rnode, CstNodeMap* map) {
  CstNode* node = cst_render_node_get_node(rnode);
  CstComNode *com_node = CST_COM_NODE(node);

  cst_com_node_set_node_map(com_node, map);
}

CstNodeMapFunc cst_com_node_get_func(SysType node_type, SysInt prop_type, SysInt data_type) {
  if(node_type == CST_TYPE_COM_NODE) {
    if(prop_type == CST_NODE_PROP_VALUE) {

      return com_node_set_value_i;
    }
  }

  if(node_type == CST_TYPE_TEXT) {
    if(prop_type == CST_NODE_PROP_VALUE) {
      if(data_type == SYS_VALUE_STRING) {

        return text_set_text_i;
      }
    }
  }

  if (sys_type_is_a(node_type, CST_TYPE_NODE)) {
    if (prop_type == CST_NODE_PROP_ACTION) {
      if(data_type == SYS_VALUE_POINTER) {

        return node_set_awatch_func_i;
      }
    }
  }

  sys_warning_N("Not support prop map for component node: %d,%d", prop_type, data_type);
  return NULL;
}

static CstLayerNode* cst_com_node_realize_i(CstNode* o, CstLayerNode *v_parent, CstComNode *com_node) {
  CstComNode *self = CST_COM_NODE(o);
  CstLayerNode* lnode = CST_NODE_CLASS(cst_com_node_parent_class)->realize(o, v_parent, self);

  return cst_component_realize(self->component, lnode, self);
}

static void cst_com_node_construct(CstComNode *self, CstComponent *comp) {
  self->component = comp;
  sys_object_ref(comp);
}

/* sys object api */
CstNode* cst_com_node_new(void) {
  return sys_object_new(CST_TYPE_COM_NODE, NULL);
}

CstNode* cst_com_node_new_I(CstComponent *comp) {
  CstNode* o = cst_com_node_new();

  cst_com_node_construct(CST_COM_NODE(o), comp);

  return o;
}

static void cst_com_node_class_init(CstComNodeClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstNodeClass *ncls = CST_NODE_CLASS(cls);

  ncls->realize = cst_com_node_realize_i;
  ocls->dispose = cst_com_node_dispose;
}

static void cst_com_node_dispose(SysObject* o) {
  CstComNode *self = CST_COM_NODE(o);

  sys_clear_pointer(&self->component, _sys_object_unref);
  if(self->values_ht) {

    sys_clear_pointer(&self->values_ht, sys_hash_table_unref);
  }

  SYS_OBJECT_CLASS(cst_com_node_parent_class)->dispose(o);
}

static void cst_com_node_init(CstComNode *self) {
  CstNode *node = CST_NODE(self);

  cst_node_set_rnode_type(node, CST_TYPE_LBOX);
}
