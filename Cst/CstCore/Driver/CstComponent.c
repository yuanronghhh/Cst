#include <CstCore/Driver/CstComponent.h>

#include <CstCore/Parser/Ast.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstLayer.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/Css/CstCssCore.h>


SYS_DEFINE_TYPE(CstComponent, cst_component, FR_TYPE_ENV);

static SysHashTable* g_component_ht;

static const SysChar* COMPONENT_BODY_NAMES[] = {
  "Data", "Component", "Layout", "Style", "Props"
};

static const SysChar* CST_COMPONENT_PROP_NAMES[] = {
  "id", "base"
};

void cst_component_setup(void) {
  sys_assert(g_component_ht == NULL);

  g_component_ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
}

void cst_component_teardown(void) {
  sys_clear_pointer(&g_component_ht, sys_hash_table_unref);
}

CstComponent* cst_component_get_g_component(const SysChar *name) {
  sys_return_val_if_fail(name != NULL, NULL);

  return sys_hash_table_lookup(g_component_ht, (const SysPointer)name);
}

SysBool cst_component_remove_g_component(const SysChar *id) {
  sys_return_val_if_fail(id != NULL, false);

  return sys_hash_table_remove(g_component_ht, (SysPointer)id);
}

void cst_component_set_g_component(CstComponent *m) {
  sys_return_if_fail(m != NULL);

  sys_hash_table_insert(g_component_ht, (SysPointer)cst_component_get_id(m), (SysPointer)m);
}

void cst_component_set_parent(CstComponent *self, CstComponent * parent) {
  sys_return_if_fail(self != NULL);

  fr_env_set_parent(FR_ENV(self), FR_ENV(parent));
}

CstComponent * cst_component_get_parent(CstComponent *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return (CstComponent *)fr_env_get_parent(FR_ENV(self));
}

CST_COMPONENT_BODY_ENUM cst_component_body_get_by_name(const SysChar * name) {
  return fr_get_type_by_name(COMPONENT_BODY_NAMES, ARRAY_SIZE(COMPONENT_BODY_NAMES), name);
}

CST_COMPONENT_PROP_ENUM cst_component_prop_get_by_name(const SysChar * name) {
  return fr_get_type_by_name(CST_COMPONENT_PROP_NAMES, ARRAY_SIZE(CST_COMPONENT_PROP_NAMES), name);
}

CstComponent* cst_component_new(void) {
  return sys_object_new(CST_TYPE_COMPONENT, NULL);
}

static SysBool component_print(CstNode *node, SysPointer user_data) {
  CstComponent *self = user_data;

  sys_debug_N("%s,%s,%s", cst_component_get_id(self), cst_node_get_name(node), cst_node_get_id(node));

  return true;
}

void cst_component_print(CstComponent* self) {
  sys_return_if_fail(self != NULL);

  cst_node_handle_node_ft_r(self->layout_node, component_print, self);
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

  return fr_env_get_r(self->value_maps_env, key);
}

void cst_component_set_value_maps_env(CstComponent *self, FREnv * value_maps_env) {
  sys_return_if_fail(self != NULL);

  self->value_maps_env = value_maps_env;
}

FREnv * cst_component_get_value_maps_env(CstComponent *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->value_maps_env;
}

void cst_component_set_value_map(CstComponent *self, CstValueMap *map) {
  sys_return_if_fail(self != NULL);

  fr_env_set(self->value_maps_env, cst_value_map_key(map), (SysPointer)map);
}

CstLayerNode* cst_component_realize(CstComponent *self, CstLayerNode *v_parent, CstComNode *com_node) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstLayerNode *root = cst_node_realize_r(self->layout_node, v_parent, com_node);

  return root;
}

SysPtrArray *cst_component_parse_base(CstComponent *self, const SysChar *base[], SysUInt len) {
  SysPtrArray * list;
  const SysChar *pname;
  CstCssGroup *ng;

  list = cst_css_group_list_new();

  for (SysUInt i = 0; i < len; i++) {
    pname = base[i];
    if (pname == NULL) { break; }

    ng = cst_component_get_css_r(self, pname);
    if (ng == NULL) {

      sys_warning_N("css \"%s\" in component \"%s\" not found", pname, self->id);
      continue;
    }

    cst_css_group_set_r(list, ng);
  }

  return list;
}
/* sys object api */
void cst_component_construct(CstComponent *self, CstComponentContext *c) {
  sys_return_if_fail(c != NULL);

  CstComponentClass *cls = CST_COMPONENT_GET_CLASS(self);

  sys_return_if_fail(cls->construct != NULL);
  cls->construct(self, c);
}

static void cst_component_construct_i(CstComponent *self, CstComponentContext *c) {
  sys_return_if_fail(self != NULL);
  SysHashTable *ht;

  CstComponent *v_pcomponent = cst_component_context_get_v_pcomponent(c);
  CstModule* v_module = cst_component_context_get_v_module(c);

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
  FR_ENV_CLASS(cst_component_parent_class)->construct(FR_ENV(self), ht, FR_ENV(v_pcomponent));

  self->layout_node = NULL;
}

static void cst_component_class_init(CstComponentClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_component_dispose;
  cls->construct = cst_component_construct_i;
}

static void cst_component_dispose(SysObject* o) {
  CstComponent *self = CST_COMPONENT(o);

  if (self->css_env) {

    sys_clear_pointer(&self->css_env, _sys_object_unref);
  }

  if (self->layout_node) {

    sys_clear_pointer(&self->layout_node, cst_node_unlink_node_r);
  }

  if (self->value_maps_env) {

    sys_clear_pointer(&self->value_maps_env, _sys_object_unref);
  }
  sys_clear_pointer(&self->id, sys_free);

  SYS_OBJECT_CLASS(cst_component_parent_class)->dispose(o);
}

static void cst_component_init(CstComponent *self) {
}
