#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Parser/Ast.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstManager.h>
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

void cst_component_set_id(CstComponent* self, const SysChar *id) {
  sys_return_if_fail(self != NULL);

  sys_assert(self->id == NULL);

  self->id = sys_strdup(id);
}

void cst_component_print(CstComponent* self) {
  sys_return_if_fail(self != NULL);

  cst_node_print_node_r(self->layout_node);
}

const SysChar* cst_component_get_id(CstComponent* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->id;
}

void cst_component_set_css(CstComponent* self, CstCssGroup *g) {
  sys_return_if_fail(self != NULL);

  cst_css_env_set(self->style_env, (SysPointer)cst_css_group_get_id(g), g);
}

SysBool cst_component_remove_css(CstComponent* self, CstCssGroup *g) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_css_env_remove(self->style_env, cst_css_group_get_id(g));
}

FREnv *cst_component_get_css_env(CstComponent* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->style_env;
}

CstCssGroup *cst_component_get_css(CstComponent* self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);

  return cst_css_env_get(CST_CSS_ENV(self->style_env), key);
}

CstCssGroup *cst_component_get_css_r(CstComponent* self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(key != NULL, NULL);

  return cst_css_env_get_r(CST_CSS_ENV(self->style_env), key);
}

SysFunc cst_component_get_function(CstComponent *self, const SysChar *func_name) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(func_name != NULL, false);

  return cst_module_get_function(self->v_module, func_name);
}

void cst_component_set_function(CstComponent *self, const SysChar *func_name, SysFunc func) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(func_name != NULL);
  sys_return_if_fail(sys_strneq(func_name, FR_FUNC_PREFIX, 2));
  sys_return_if_fail(func != NULL);

  cst_module_set_function(self->v_module, func_name, func);
}

CstValueMap *cst_component_get_value_map(CstComponent *self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);

  return fr_env_get_r(self->prop_maps_env, key);
}

void cst_component_set_value_map(CstComponent *self, CstValueMap *map) {
  sys_return_if_fail(self != NULL);

  fr_env_set(self->prop_maps_env, cst_value_map_key(map), (SysPointer)map);
}

CstLayerNode* cst_component_realize_full(CstComponent* self, CstLayerNode* parent, CstLayout* layout) {

  return cst_node_realize_r(self->layout_node, parent, self->v_module, layout);
}

SysBool component_realize_cb(CstNode *node, CstNodeRealizer *pass) {
  CstLayerNode *lnode = cst_node_realizer_realize(pass, node);
  return true;
}

CstLayerNode* cst_component_realize(CstComponent *self, CstNodeRealizer *pass, CstLayout* layout) {
  sys_return_val_if_fail(self != NULL, NULL);

  return fr_node_handle_node_ff_r(self->layout_node, (FRNodeFunc)component_realize_cb, (SysPointer)pass);
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
  fr_env_set_parent(self->style_env, pself->style_env);
}

static void cst_component_construct_i(CstComponent *self, CstComponentBuilder *builder) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(builder != NULL);
  SysHashTable *ht;
  CstComponent *pcomp;

  pcomp = cst_component_builder_get_v_parent(builder);

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
  FR_ENV_CLASS(cst_component_parent_class)->construct(FR_ENV(self), ht, FR_ENV(pcomp));

  cst_component_builder_build_component(builder, self);

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
  self->prop_maps_env = fr_env_new_I(ht, NULL);

  self->style_env = cst_css_env_new_I(NULL);
  self->layout_node = NULL;
}

void cst_component_set_v_module(CstComponent *self, CstModule * v_module) {
  sys_return_if_fail(self != NULL);

  self->v_module = v_module;
}

CstModule * cst_component_get_v_module(CstComponent *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_module;
}

static void cst_component_class_init(CstComponentClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = cst_component_construct_i;
  ocls->dispose = cst_component_dispose;
}

static void cst_component_dispose(SysObject* o) {
  CstComponent *self = CST_COMPONENT(o);

  sys_clear_pointer(&self->style_env, _sys_object_unref);
  sys_clear_pointer(&self->prop_maps_env, _sys_object_unref);

  cst_node_unlink_node_r(self->layout_node);

  sys_clear_pointer(&self->id, sys_free);

  SYS_OBJECT_CLASS(cst_component_parent_class)->dispose(o);
}

static void cst_component_init(CstComponent *self) {
}
