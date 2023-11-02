#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Parser/Ast.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstLayer.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/Css/CstCssCore.h>


SYS_DEFINE_TYPE(CstComponent, cst_component, FR_TYPE_ENV);


static const SysChar* COMPONENT_BODY_NAMES[] = {
  "Data", "Component", "Layout", "Style", "Props"
};

static const SysChar* CST_COMPONENT_PROP_NAMES[] = {
  "id", "base"
};

CST_COMPONENT_BODY_ENUM cst_component_body_get_by_name(const SysChar * name) {
  return fr_get_type_by_name(COMPONENT_BODY_NAMES, ARRAY_SIZE(COMPONENT_BODY_NAMES), name);
}

CST_COMPONENT_PROP_ENUM cst_component_prop_get_by_name(const SysChar * name) {
  return fr_get_type_by_name(CST_COMPONENT_PROP_NAMES, ARRAY_SIZE(CST_COMPONENT_PROP_NAMES), name);
}

CstComponent* cst_component_new(void) {
  return sys_object_new(CST_TYPE_COMPONENT, NULL);
}

void cst_component_set_layout_node(CstComponent *self, CstNode *node) {
  sys_return_if_fail(self != NULL);

  self->layout_node = node;
}

CstNode *cst_component_get_layout_node(CstComponent *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->layout_node;
}

static SysBool component_print(FRNode *o, SysPointer user_data) {
  CstNode *node = CST_NODE(o);
  CstComponent *self = user_data;

  sys_debug_N("%s,%s,%s", cst_component_get_id(self), cst_node_get_name(node), cst_node_get_id(node));

  return true;
}

void cst_component_print(CstComponent* self) {
  sys_return_if_fail(self != NULL);

  fr_node_handle_bfs_r(FR_NODE(self->layout_node), component_print, self);
}

void cst_component_set_id(CstComponent* self, const SysChar *id) {
  sys_return_if_fail(self != NULL);

  sys_assert(self->id == NULL);

  self->id = sys_strdup(id);
}

const SysChar* cst_component_get_id(CstComponent* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->id;
}

void cst_component_set_css(CstComponent* self, CstCssGroup *g) {
  sys_return_if_fail(self != NULL);

  fr_env_set(self->css_env, (SysPointer)cst_css_group_get_id(g), g);
}

SysBool cst_component_remove_css(CstComponent* self, CstCssGroup *g) {
  sys_return_val_if_fail(self != NULL, false);

  return fr_env_remove(self->css_env, cst_css_group_get_id(g));
}

void cst_component_set_css_env(CstComponent *self, FREnv * css_env) {
  sys_return_if_fail(self != NULL);

  self->css_env = css_env;
}

FREnv * cst_component_get_css_env(CstComponent *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->css_env;
}

CstCssGroup *cst_component_get_css(CstComponent* self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);

  return fr_env_get(self->css_env, key);
}

CstCssGroup *cst_component_get_css_r(CstComponent* self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(key != NULL, NULL);

  return fr_env_get_r(self->css_env, key);
}

CstValueMap *cst_component_get_value_map(CstComponent *self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);

  return fr_env_get_r(self->prop_maps_env, key);
}

void cst_component_set_prop_maps_env(CstComponent *self, FREnv * prop_maps_env) {
  sys_return_if_fail(self != NULL);

  self->prop_maps_env = prop_maps_env;
}

FREnv * cst_component_get_prop_maps_env(CstComponent *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->prop_maps_env;
}

void cst_component_set_value_map(CstComponent *self, CstValueMap *map) {
  sys_return_if_fail(self != NULL);

  fr_env_set(self->prop_maps_env, cst_value_map_key(map), (SysPointer)map);
}

CstLayerNode* cst_component_realize(CstComponent *self, CstLayerNode *v_parent, CstComNode *com_node) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstLayerNode *root = cst_node_realize_r(self->layout_node, v_parent, com_node);

  return root;
}

SysPtrArray *cst_component_parse_base(CstComponent *self, const SysChar *v_base[], SysUInt len) {
  SysPtrArray * list = cst_css_group_list_new();

  for (SysUInt i = 0; i < len; i++) {
    pname = v_base[i];
    if (pname == NULL) { break; }

    ng = cst_component_get_css_r(component, pname);
    if (ng == NULL) {

      sys_warning_N("css \"%s\" in component \"%s\" not found", pname, component->id);
      continue;
    }

    cst_css_group_set_r(list, ng);
  }

  return list;
}
/* sys object api */
void cst_component_construct(CstComponent *self, CstComponentBuilder *builder) {
  sys_return_if_fail(builder != NULL);

  CstComponentClass *cls = CST_COMPONENT_GET_CLASS(self);

  sys_return_if_fail(cls->construct != NULL);
  cls->construct(self, builder);
}

void cst_component_bind_parent(CstComponent *self, CstComponent *pself) {

  fr_env_set_parent(self->prop_maps_env, pself->prop_maps_env);
  fr_env_set_parent(self->css_env, pself->css_env);
}

FREnv *cst_component_new_prop_maps_env(FREnv *parent) {
  SysHashTable *ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);

  return fr_env_new_I(ht, parent);
}

static void cst_component_construct_i(CstComponent *self, CstComponentBuilder *builder) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(builder != NULL);
  SysHashTable *ht;
  CstComponent *pcomp;

  pcomp = cst_component_builder_get_v_parent(builder);

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
  FR_ENV_CLASS(cst_component_parent_class)->construct(FR_ENV(self), ht, FR_ENV(pcomp));

  cst_component_builder_build(builder, self);
}

static void cst_component_class_init(CstComponentClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = cst_component_construct_i;
  ocls->dispose = cst_component_dispose;
}

static void cst_component_dispose(SysObject* o) {
  CstComponent *self = CST_COMPONENT(o);

  sys_clear_pointer(&self->css_env, _sys_object_unref);
  sys_clear_pointer(&self->prop_maps_env, _sys_object_unref);

  cst_node_unlink_node_r(self->layout_node);

  sys_clear_pointer(&self->id, sys_free);

  SYS_OBJECT_CLASS(cst_component_parent_class)->dispose(o);
}

static void cst_component_init(CstComponent *self) {
}
