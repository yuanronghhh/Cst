#include <CstCore/Driver/CstComponentBuilder.h>

#include <CstCore/Front/Common/CstLBox.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/Css/CstCssEnv.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstNode.h>


SYS_DEFINE_TYPE(CstComponentBuilder, cst_component_builder, SYS_TYPE_OBJECT);


void cst_component_builder_set_base_name(CstComponentBuilder *self, SysChar *v_base_name) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_base_name != NULL);

  sys_assert(self->v_base_name == NULL);

  self->v_base_name = sys_strdup(v_base_name);
}

void cst_component_builder_set_id(CstComponentBuilder *self, const SysChar * id) {
  sys_return_if_fail(self != NULL);

  if(self->id) {
    sys_clear_pointer(&self->id, sys_free);
  }

  self->id = sys_strdup(id);
}

const SysChar * cst_component_builder_get_id(CstComponentBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->id;
}

void cst_component_builder_set_v_parent(CstComponentBuilder *self, CstComponent * v_parent) {
  sys_return_if_fail(self != NULL);

  self->v_parent = v_parent;
}

CstComponent * cst_component_builder_get_v_parent(CstComponentBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_parent;
}

CstValueMap *cst_component_builder_get_value_map(CstComponentBuilder *self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);

  return fr_env_get_r(self->prop_maps_env, key);
}

void cst_component_builder_set_value_map(CstComponentBuilder *self, CstValueMap *map) {
  sys_return_if_fail(self != NULL);

  fr_env_set(self->prop_maps_env, cst_value_map_key(map), (SysPointer)map);
}

void cst_component_builder_set_css(CstComponentBuilder* self, CstCssGroup *g) {
  sys_return_if_fail(self != NULL);

  fr_env_set(self->css_env, (SysPointer)cst_css_group_get_id(g), g);
}

void cst_component_builder_set_css_env(CstComponentBuilder *self, FREnv * css_env) {
  sys_return_if_fail(self != NULL);

  self->css_env = css_env;
}

FREnv * cst_component_builder_get_css_env(CstComponentBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->css_env;
}

SysBool cst_component_builder_remove_css(CstComponentBuilder* self, CstCssGroup *g) {
  sys_return_val_if_fail(self != NULL, false);

  return fr_env_remove(self->css_env, cst_css_group_get_id(g));
}

void cst_component_builder_parse_i(CstComponentBuilder *self, Component *ast) {
  sys_return_if_fail(self != NULL);

  ast_component_body_parse(ast, self);
}

void cst_component_builder_build_i(CstBuilder *o, SysObject *v) {
  sys_return_if_fail(o != NULL);
  CstComponent *comp = CST_COMPONENT(v);

  cst_component_set_id(comp, self->id);
  cst_component_set_prop_maps_env(comp, self->prop_maps_env);
  cst_component_set_css_env(comp, self->css_env);
}

/* object api */
static void cst_component_builder_dispose(SysObject* o) {
  CstComponentBuilder *self = CST_COMPONENT_BUILDER(o);

  if(self->id) {
    sys_clear_pointer(&self->id, sys_free);
  }

  if(self->v_base_name) {
    sys_clear_pointer(&self->v_base_name, sys_free);
  }

  SYS_OBJECT_CLASS(cst_component_builder_parent_class)->dispose(o);
}

CstBuilder *cst_component_builder_new(void) {

  return sys_object_new(CST_TYPE_COMPONENT_BUILDER, NULL);
}

CstComponentBuilder *cst_component_builder_new_I(CstContext *c, ) {
  CstBuilder *o = cst_component_builder_new();

  cst_component_builder_construct(o, v_module, NULL, NULL);

  return o;
}

static void cst_component_builder_class_init(CstComponentBuilderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstBuilderClass *bcls = CST_BUILDER_CLASS(cls);

  ocls->dispose = cst_component_builder_dispose;

  bcls->construct = cst_component_builder_construct;
  bcls->parse = cst_component_builder_parse_i;
  bcls->build = cst_component_builder_build_i;
}

static void cst_component_builder_init(CstComponentBuilder *self) {
}
