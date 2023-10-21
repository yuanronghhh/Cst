#include <CstCore/Driver/CstNodeMap.h>
#include <CstCore/Front/CstFrontCore.h>
#include <CstCore/Front/Common/CstComNode.h>
#include <CstCore/Driver/CstPropPair.h>
#include <CstCore/Driver/CstRenderNode.h>


SYS_DEFINE_TYPE(CstNodeMap, cst_node_map, SYS_TYPE_OBJECT);

CstNodeMap* cst_node_map_new(void) {
  return sys_object_new(CST_TYPE_NODE_MAP, NULL);
}

void cst_node_map_construct(CstNodeMap *self, CstPropMap *prop_map, SysInt prop_type, const SysChar *prop_name) {
  sys_assert(prop_type > 0 && "prop_type > 0 failed, see CST_NODE_PROP_ENUM");

  self->prop_map = prop_map;
  self->prop_type = prop_type;
  self->prop_name = sys_strdup(prop_name);
  self->value = NULL;

  sys_object_ref(prop_map);
}

CstNodeMap* cst_node_map_new_I(CstPropMap *prop_map, SysInt prop_type, const SysChar *prop_name) {
  CstNodeMap * o = cst_node_map_new();

  cst_node_map_construct(o, prop_map, prop_type, prop_name);

  return o;
}

CstNodeMap* cst_node_map_clone(CstNodeMap* o) {
  sys_return_val_if_fail(o != NULL, NULL);

  SysType type = sys_type_from_instance(o);
  CstNodeMap *n = sys_object_new(type, NULL);

  n->prop_map = o->prop_map;
  sys_object_ref(n->prop_map);

  n->node_type = o->node_type;
  n->prop_type = o->prop_type;
  n->prop_name = sys_strdup(o->prop_name);

  n->func = o->func;

  sys_assert(o->value == NULL);
  n->value = o->value;

  return n;
}

void cst_node_map_bind(CstNodeMap *self, CstComNode *com_node, CstRenderNode *rnode) {
  sys_return_if_fail(self != NULL);

  const SysChar *bind_var = cst_prop_map_key(self->prop_map);
  SysInt data_type = cst_prop_map_prop_data_type(self->prop_map);

  CstPropValue *pvalue = cst_com_node_get_value(com_node, bind_var);
  if (pvalue == NULL) {
    sys_warning_N("Not found key \"%s\" in component node \"%s\"", 
      bind_var, cst_node_get_name(CST_NODE(com_node)));
    return;
  }

  self->node_type = cst_render_node_get_node_type(rnode);
  self->value = pvalue;
  sys_value_ref(pvalue);

  self->func = cst_com_node_get_func(self->node_type, self->prop_type, data_type);
  if (self->func) {
    self->func(rnode, self->prop_name, bind_var, self->value);
  }
}

void cst_node_map_calc(CstNodeMap *self, CstNode *v_node) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_node != NULL);

  const SysChar *key = cst_prop_map_key(self->prop_map);

  SysType type = sys_type_from_instance(v_node);

  if(self->node_type != type) {
    sys_warning_N("Not same node type: \"%s\"", key);
    return;
  }

  self->func(v_node, self->prop_name, key, self->value);
}

const SysChar * cst_node_map_get_prop_name(CstNodeMap *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return cst_prop_map_key(self->prop_map);
}

/* object api */
static void cst_node_map_init(CstNodeMap *self) {
}

static void cst_node_map_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  CstNodeMap *self = CST_NODE_MAP(o);
  sys_object_unref(self->prop_map);
  sys_clear_pointer(&self->prop_name, sys_free);

  // meta node map has no value data
  if (self->value) {
    sys_clear_pointer(&self->value, sys_value_unref);
  }

  SYS_OBJECT_CLASS(cst_node_map_parent_class)->dispose(o);
}

static void cst_node_map_class_init(CstNodeMapClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_node_map_dispose;
}
