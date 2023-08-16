#include <CstCore/Front/Common/CstNodeMap.h>
#include <CstCore/Front/CstFrontCore.h>
#include <CstCore/Front/Common/CstPropPair.h>
#include <CstCore/Front/Common/CstComNode.h>


struct _CstNodeMapPrivate {
  CstPropMap *prop_map;
  SysType node_type;

  /* CST_TYPE_NODE_PROP */
  SysInt prop_type;
  SysChar* prop_name;
  SysValue *value;
  CstNodeMapFunc func;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstNodeMap, cst_node_map, SYS_TYPE_OBJECT);

CstNodeMap* cst_node_map_new(void) {
  return sys_object_new(CST_TYPE_NODE_MAP, NULL);
}

void cst_node_map_construct(CstNodeMap *self, CstPropMap *prop_map, SysInt prop_type, const SysChar *prop_name) {
  CstNodeMapPrivate* priv = self->priv;

  sys_assert(prop_type > 0 && "prop_type > 0 failed, see CST_NODE_PROP_ENUM");

  priv->prop_map = prop_map;
  priv->prop_type = prop_type;
  priv->prop_name = sys_strdup(prop_name);
  priv->value = NULL;

  sys_object_ref(prop_map);
}

CstNodeMap* cst_node_map_new_I(CstPropMap *prop_map, SysInt prop_type, const SysChar *prop_name) {
  CstNodeMap * o = cst_node_map_new();

  cst_node_map_construct(o, prop_map, prop_type, prop_name);

  return o;
}

CstNodeMap* cst_node_map_clone(CstNodeMap* map) {
  sys_return_val_if_fail(map != NULL, NULL);

  SysType type = sys_type_from_instance(map);
  CstNodeMap *nmap = sys_object_new(type, NULL);

  CstNodeMapPrivate *npriv = nmap->priv;
  CstNodeMapPrivate *opriv = map->priv;

  npriv->prop_map = opriv->prop_map;
  sys_object_ref(npriv->prop_map);

  npriv->node_type = opriv->node_type;
  npriv->prop_type = opriv->prop_type;
  npriv->prop_name = sys_strdup(opriv->prop_name);

  npriv->func = opriv->func;

  sys_assert(opriv->value == NULL);
  npriv->value = opriv->value;

  return nmap;
}

void cst_node_map_bind(CstNodeMap *self, CstComNode *com_node, CstNode *v_node) {
  sys_return_if_fail(self != NULL);

  CstNodeMapPrivate* priv = self->priv;
  const SysChar *bind_var = cst_prop_map_key(priv->prop_map);
  SysInt data_type = cst_prop_map_prop_data_type(priv->prop_map);
  SysType node_type = sys_type_from_instance(v_node);

  CstPropValue *pvalue = cst_com_node_get_value(com_node, bind_var);
  if (pvalue == NULL) {
    sys_warning_N("Not found key \"%s\" in component node \"%s\"", 
      bind_var, cst_node_get_name(CST_NODE(com_node)));
    return;
  }

  priv->node_type = node_type;
  priv->value = pvalue;
  sys_value_ref(pvalue);

  priv->func = cst_com_node_get_func(node_type, priv->prop_type, data_type);
  if (priv->func) {
    priv->func(v_node, priv->prop_name, bind_var, priv->value);
  }
}

void cst_node_map_calc(CstNodeMap *self, CstNode *v_node) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_node != NULL);

  CstNodeMapPrivate* priv = self->priv;
  const SysChar *key = cst_prop_map_key(priv->prop_map);

  SysType type = sys_type_from_instance(v_node);

  if(priv->node_type != type) {
    sys_warning_N("Not same node type: \"%s\"", key);
    return;
  }

  priv->func(v_node, priv->prop_name, key, priv->value);
}

const SysChar * cst_node_map_get_prop_name(CstNodeMap *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstNodeMapPrivate* priv = self->priv;

  return cst_prop_map_key(priv->prop_map);
}

/* object api */
static void cst_node_map_init(CstNodeMap *self) {
  self->priv = cst_node_map_get_private(self);
}

static void cst_node_map_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  CstNodeMap *self = CST_NODE_MAP(o);
  CstNodeMapPrivate* priv = self->priv;

  sys_object_unref(priv->prop_map);
  sys_clear_pointer(&priv->prop_name, sys_free);

  // meta node map has no value data
  if (priv->value) {
    sys_clear_pointer(&priv->value, sys_value_unref);
  }

  SYS_OBJECT_CLASS(cst_node_map_parent_class)->dispose(o);
}

static void cst_node_map_class_init(CstNodeMapClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_node_map_dispose;
}
