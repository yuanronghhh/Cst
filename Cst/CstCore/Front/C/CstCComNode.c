#include <CstCore/Front/Common/CstComNode.h>
#include <CstCore/Front/Common/CstNodeMapCore.h>
#include <CstCore/Front/CstFrontCore.h>
#include <CstCore/Front/Common/CstComponent.h>
#include <CstCore/Front/C/CstCComNode.h>
#include <CstCore/Driver/CstLayer.h>


struct _CstComNodePrivate {
  CstComponent *component;

  SysHashTable *values_ht;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstComNode, cst_com_node, CST_TYPE_NODE);


CstNode* cst_com_node_new(void) {
  return sys_object_new(CST_TYPE_COM_NODE, NULL);
}

CstNode* cst_com_node_new_with_component(CstComponent *comp) {
  sys_return_val_if_fail(comp != NULL, NULL);

  CstNode *nnode = cst_com_node_new();
  CstComNode *self = CST_COM_NODE(nnode);
  CstComNodePrivate *priv = self->priv;

  priv->component = comp;
  sys_object_ref(comp);

  return nnode;
}

CstComponent *cst_com_node_get_component(CstComNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstComNodePrivate *priv = self->priv;

  return priv->component;
}

SysValue * cst_com_node_get_value(CstComNode * self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstComNodePrivate *priv = self->priv;

  return sys_hash_table_lookup(priv->values_ht, (SysPointer)key);
}

void com_node_set_value(CstComNode *self, const SysChar *key, SysValue * value) {
  sys_return_if_fail(self != NULL);

  CstComNodePrivate *priv = self->priv;

  sys_hash_table_insert(priv->values_ht, (SysPointer)sys_strdup(key), value);
}

static void text_set_text_i(CstNode *v_node, const SysChar *key, SysValue *value) {
  CstText *text = CST_TEXT(v_node);

  cst_text_set_text(text, sys_value_v_string(value));
}

static void com_node_set_value_i(CstNode *v_node, const SysChar *key, SysValue *value) {
  CstComNode *com_node = CST_COM_NODE(v_node);

  com_node_set_value(com_node, key, value);
}

CstNodeMapFunc cst_com_node_get_func(SysType node_type, SysInt prop_type, SysInt data_type) {
  if(data_type == SYS_STRING) {
    if(node_type == CST_TYPE_TEXT && prop_type == CST_NODE_PROP_VALUE) {
      return text_set_text_i;
    }

    if(node_type == CST_TYPE_COM_NODE && prop_type == CST_NODE_PROP_VALUE) {
      return com_node_set_value_i;
    }
  }

  return NULL;
}

/* sys object api */
void cst_com_node_construct_i(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props) {
  CST_NODE_CLASS(cst_com_node_parent_class)->construct(v_module, v_component, v_parent, v_node, v_props);

}

static CstNode* cst_com_node_realize_i(CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render) {
  sys_return_val_if_fail(v_module != NULL, NULL);
  sys_return_val_if_fail(v_parent != NULL, NULL);
  sys_return_val_if_fail(v_node != NULL, NULL);

  CstComNode *self = CST_COM_NODE(v_node);
  CstComNodePrivate *priv = self->priv;

  cst_component_realize_full(v_module, priv->component, v_parent, self, v_render);

  return NULL;
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
  CstComNodePrivate *priv = self->priv;

  sys_object_unref(priv->component);
  sys_hash_table_unref(priv->values_ht);

  SYS_OBJECT_CLASS(cst_com_node_parent_class)->dispose(o);
}

static void cst_com_node_init(CstComNode *self) {
  CstComNodePrivate *priv = self->priv = cst_com_node_get_private(self);
  SysHashTable *ht;

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, sys_free, (SysDestroyFunc)sys_value_unref);

  priv->values_ht = ht;
}
