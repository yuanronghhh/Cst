#include <CstCore/Driver/CstNodeBuilder.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstNode.h>


SYS_DEFINE_TYPE(CstNodeBuilder, cst_node_builder, SYS_TYPE_OBJECT);


static SysChar *node_builder_new_id(CstNode* node, CstModule *v_module) {
  SysChar *nid = NULL;
  SysUInt mid;
  SysUInt ccount = 0;

  if (v_module) {
    mid = cst_module_get_hashcode(v_module);
    ccount = cst_module_count(v_module);

  } else {
    mid = sys_str_hash((SysPointer)"root-node");
  }

  nid = sys_strdup_printf("id.%u.%u", mid, ccount);

  return nid;
}

void cst_node_builder_build_props(CstNodeBuilder *self, CstNode *v_node) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_node != NULL);

  SysChar** v_base = self->v_base;
  SysInt v_base_len = self->v_base_len;
  SysChar *id;

  if (self->v_id) {

    cst_node_set_id(v_node, self->v_id);
  } else {

    id = node_builder_new_id(v_node, self->v_module);
    cst_node_set_id(v_node, id);
    sys_free_N(id);
  }

  cst_node_set_node_maps_list(v_node, self->v_node_maps);
  cst_node_set_awatch_list(v_node, self->v_awatches);

  // LBody has no parent component.
  cst_node_set_css_props(v_node, self->v_component, (const SysChar**)v_base, v_base_len);

  switch (self->v_position) {
    case CST_LAYER_BOX:
    case CST_LAYER_ABS:
      break;
    default:
      sys_abort_N("node builder position must be set: %s,%s", cst_node_get_name(v_node), cst_node_get_id(v_node));
      break;
  }

  cst_node_set_position(v_node, self->v_position);
}

void cst_node_builder_build(CstNodeBuilder *self, CstNode *v_node) {

  cst_node_builder_build_props(self, v_node);
}

CstModule* cst_node_builder_get_module(CstNodeBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_module;
}

CstNode* cst_node_builder_get_pnode(CstNodeBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_pnode;
}

CstComponent* cst_node_builder_get_component(CstNodeBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_component;
}

const SysChar* cst_node_builder_get_value(CstNodeBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_value;
}

void cst_node_builder_set_base(CstNodeBuilder *self, SysChar *v_base[], SysUInt v_len) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_base != NULL);

  self->v_base = v_base;
  self->v_base_len = v_len;
}

void cst_node_builder_set_id(CstNodeBuilder *self, SysChar *v_id) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_id != NULL);

  self->v_id = sys_strdup(v_id);
}

void cst_node_builder_set_value(CstNodeBuilder *self, SysChar *v_value) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_value != NULL);

  self->v_value = sys_strdup(v_value);
}

void cst_node_builder_set_position(CstNodeBuilder *self, SysInt v_position) {
  sys_return_if_fail(self != NULL);

  self->v_position = v_position;
}

void cst_node_builder_set_label(CstNodeBuilder *self, SysChar *v_label) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_label != NULL);

  self->v_label = sys_strdup(v_label);
}

void cst_node_builder_add_node_maps(CstNodeBuilder *self, CstNodeMap* map) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(map != NULL);

  self->v_node_maps = sys_list_prepend(self->v_node_maps, map);
}

void cst_node_builder_add_awatches(CstNodeBuilder *self, FRAWatch* map) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(map != NULL);

  self->v_awatches = sys_list_prepend(self->v_awatches, map);
}

/* object api */
static void cst_node_builder_dispose(SysObject* o) {
  CstNodeBuilder *self = CST_NODE_BUILDER(o);

  SYS_OBJECT_CLASS(cst_node_builder_parent_class)->dispose(o);
}

CstNodeBuilder *cst_node_builder_new(void) {
  return sys_object_new(CST_TYPE_NODE_BUILDER, NULL);
}

void cst_node_builder_construct(CstNodeBuilder *o, CstModule* v_module, CstComponent* v_component, CstNode* v_pnode) {
  o->v_module = v_module;
  o->v_component = v_component;
  o->v_pnode = v_pnode;
  o->v_position = CST_LAYER_BOX;
}

CstNodeBuilder *cst_node_builder_new_I(CstModule* v_module, CstComponent* v_component, CstNode* v_pnode) {
  CstNodeBuilder *o = cst_node_builder_new();

  cst_node_builder_construct(o, v_module, v_component, v_pnode);

  return o;
}

CstNodeBuilder *cst_node_builder_new_simple(CstModule* v_module, CstNode* v_pnode) {
  CstNodeBuilder *o = cst_node_builder_new();

  cst_node_builder_construct(o, v_module, NULL, v_pnode);

  return o;
}

static void cst_node_builder_class_init(CstNodeBuilderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_node_builder_dispose;
  cls->construct = cst_node_builder_construct;
}

static void cst_node_builder_init(CstNodeBuilder *self) {
}
