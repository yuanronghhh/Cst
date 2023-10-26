#include <CstCore/Driver/CstNodeMap.h>
#include <CstCore/Front/CstFrontCore.h>
#include <CstCore/Front/Common/CstComNode.h>
#include <CstCore/Driver/CstRenderNode.h>


SYS_DEFINE_TYPE(CstNodeMap, cst_node_map, FR_TYPE_PAIR);

void cst_node_map_construct(CstNodeMap *self, CstValueMap *value_map, SysInt prop_type, const SysChar *prop_name, SysPointer value) {
  sys_assert(prop_type > 0 && "prop_type > 0 failed, see CST_NODE_PROP_ENUM");

  FR_PAIR_CLASS(cst_node_map_parent_class)->construct(FR_PAIR(self), prop_name, value);

  self->value_map = value_map;
  self->prop_type = prop_type;

  sys_object_ref(value_map);
}

SysObject* cst_node_map_dclone_i(SysObject* o) {
  sys_return_val_if_fail(o != NULL, NULL);

  SysObject *n = SYS_OBJECT_CLASS(cst_node_map_parent_class)->dclone(o);

  CstNodeMap *oself = CST_NODE_MAP(o);
  CstNodeMap *nself = CST_NODE_MAP(n);

  nself->value_map = oself->value_map;
  sys_object_ref(nself->value_map);

  nself->node_type = oself->node_type;
  nself->prop_type = oself->prop_type;
  nself->func = oself->func;

  return n;
}

void cst_node_map_bind(CstNodeMap *self, CstComNode *com_node, CstRenderNode *rnode) {
  sys_return_if_fail(self != NULL);

  FRPair* pair;
  CstNodeMap* map;
  const SysChar* bind_var;
  SysInt data_type;
  CstNode* node;

  bind_var = cst_value_map_key(self->value_map);
  data_type = cst_value_map_prop_data_type(self->value_map);
  node = cst_render_node_get_node(rnode);

  map = cst_com_node_get_node_map(com_node, bind_var);
  if (map == NULL) {
    sys_warning_N("Not found key \"%s\" in component node \"%s\"", 
      bind_var, cst_node_get_name(CST_NODE(com_node)));
    return;
  }

  pair = FR_PAIR(map);
  self->node_type = sys_type_from_instance(node);
  sys_object_ref(rnode);

  self->func = cst_com_node_get_func(self->node_type, self->prop_type, data_type);
  if (self->func) {
    self->func(rnode, map);
  }
}

void cst_node_map_variable_handle(CstNodeMap* self, const SysChar *variable) {
  sys_return_if_fail(self != NULL);
}

/* object api */
CstNodeMap* cst_node_map_new(void) {
  return sys_object_new(CST_TYPE_NODE_MAP, NULL);
}

CstNodeMap* cst_node_map_new_I(CstValueMap *value_map, SysInt prop_type, const SysChar *prop_name, SysPointer value) {
  CstNodeMap * o = cst_node_map_new();

  cst_node_map_construct(o, value_map, prop_type, prop_name, value);

  return o;
}

static void cst_node_map_init(CstNodeMap *self) {
}

static void cst_node_map_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  CstNodeMap *self = CST_NODE_MAP(o);
  sys_object_unref(self->value_map);

  SYS_OBJECT_CLASS(cst_node_map_parent_class)->dispose(o);
}

static void cst_node_map_class_init(CstNodeMapClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_node_map_dispose;
  ocls->dclone = cst_node_map_dclone_i;
}
