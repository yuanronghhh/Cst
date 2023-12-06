#include <CstCore/Driver/CstComponentContext.h>

#include <CstCore/Front/Common/CstLBox.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/Css/CstCssEnv.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstNode.h>


SYS_DEFINE_TYPE(CstComponentContext, cst_component_context, SYS_TYPE_OBJECT);


/* builder */
SysBool cst_component_context_parse_base(CstComponentContext *self, const SysChar *base) {
  sys_return_val_if_fail(self != NULL, false);

  CstComponent *c = cst_component_get_g_component(base);
  if(c == NULL) {

    sys_warning_N("%s is not a component", base);
    return false;
  }
  self->base_component = c;
  sys_object_ref(c);

  return true;
}

void cst_component_context_set_id(CstComponentContext *self, const SysChar * id) {
  sys_return_if_fail(self != NULL);

  if(self->id) {
    sys_clear_pointer(&self->id, sys_free);
  }

  self->id = sys_strdup(id);
}

const SysChar * cst_component_context_get_id(CstComponentContext *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->id;
}

void cst_component_context_set_v_parent(CstComponentContext *self, CstComponent * v_parent) {
  sys_return_if_fail(self != NULL);

  self->v_parent = v_parent;
}

CstComponent * cst_component_context_get_v_parent(CstComponentContext *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_parent;
}

CstValueMap *cst_component_context_get_value_map(CstComponentContext *self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);

  return fr_env_get_r(self->value_maps_env, key);
}

void cst_component_context_set_value_map(CstComponentContext *self, CstValueMap *map) {
  sys_return_if_fail(self != NULL);

  fr_env_set(self->value_maps_env, cst_value_map_key(map), (SysPointer)map);
}

void cst_component_context_set_css(CstComponentContext* self, CstCssGroup *g) {
  sys_return_if_fail(self != NULL);

  fr_env_set(self->css_env, (SysPointer)cst_css_group_get_id(g), g);
}

void cst_component_context_set_css_env(CstComponentContext *self, FREnv * css_env) {
  sys_return_if_fail(self != NULL);

  self->css_env = css_env;
}

FREnv * cst_component_context_get_css_env(CstComponentContext *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->css_env;
}

SysBool cst_component_context_remove_css(CstComponentContext* self, CstCssGroup *g) {
  sys_return_val_if_fail(self != NULL, false);

  return fr_env_remove(self->css_env, cst_css_group_get_id(g));
}

void cst_component_context_build(CstComponentContext *self, CstComponent *o) {
  sys_return_if_fail(self != NULL);

  cst_component_set_id(o, self->id);
  cst_component_set_value_maps_env(o, self->value_maps_env);
  sys_object_ref(self->value_maps_env);

  cst_component_set_css_env(o, self->css_env);
  sys_object_ref(self->css_env);
}

void cst_component_context_set_v_module(CstComponentContext *self, CstModule * v_module) {
  sys_return_if_fail(self != NULL);

  self->v_module = v_module;
}

CstModule * cst_component_context_get_v_module(CstComponentContext *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_module;
}

void cst_component_context_set_v_pcomponent(CstComponentContext *self, CstComponent * v_pcomponent) {
  sys_return_if_fail(self != NULL);

  self->v_pcomponent = v_pcomponent;
}

CstComponent * cst_component_context_get_v_pcomponent(CstComponentContext *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_pcomponent;
}

/* object api */
static void cst_component_context_dispose(SysObject* o) {
  CstComponentContext *self = CST_COMPONENT_CONTEXT(o);

  if(self->id) {

    sys_clear_pointer(&self->id, sys_free);
  }

  if(self->base_component) {

    sys_clear_pointer(&self->base_component, _sys_object_unref);
  }

  sys_clear_pointer(&self->value_maps_env, _sys_object_unref);
  sys_clear_pointer(&self->css_env, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_component_context_parent_class)->dispose(o);
}

CstComponentContext *cst_component_context_new(void) {

  return sys_object_new(CST_TYPE_COMPONENT_CONTEXT, NULL);
}

static void cst_component_context_class_init(CstComponentContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_component_context_dispose;
}

static void cst_component_context_init(CstComponentContext *self) {
  self->css_env = cst_css_env_new_I(NULL);
  self->value_maps_env = cst_value_map_new_env(NULL);
}
