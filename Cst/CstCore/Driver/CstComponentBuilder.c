#include <CstCore/Driver/CstComponentBuilder.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstComponent.h>


SYS_DEFINE_TYPE(CstComponentBuilder, cst_component_builder, SYS_TYPE_OBJECT);


void cst_component_builder_build(CstComponentBuilder *self, CstComponent *v_component) {
}

void cst_component_builder_set_base_name(CstComponentBuilder *self, SysChar *v_base_name) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_base_name != NULL);

  sys_assert(self->v_base_name == NULL);

  self->v_base_name = sys_strdup(v_base_name);
}

void cst_component_builder_set_id(CstComponentBuilder *self, const SysChar *v_id) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_id != NULL);

  sys_assert(self->v_id == NULL);

  self->v_id = sys_strdup(v_id);
}

const SysChar *cst_component_builder_get_id(CstComponentBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_id;
}

void cst_component_builder_set_v_parent(CstComponentBuilder *self, CstComponent * v_parent) {
  sys_return_if_fail(self != NULL);

  self->v_parent = v_parent;
}

CstComponent * cst_component_builder_get_v_parent(CstComponentBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_parent;
}

void cst_component_builder_set_v_module(CstComponentBuilder *self, CstModule * v_module) {
  sys_return_if_fail(self != NULL);

  self->v_module = v_module;
}

CstModule * cst_component_builder_get_v_module(CstComponentBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_module;
}

void cst_component_builder_set_v_render(CstComponentBuilder *self, CstRender * v_render) {
  sys_return_if_fail(self != NULL);

  self->v_render = v_render;
}

CstRender * cst_component_builder_get_v_render(CstComponentBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_render;
}

void cst_component_builder_build_component(CstComponentBuilder *self, CstComponent *o) {
  sys_return_if_fail(self != NULL);

  cst_component_set_id(o, self->v_id);
}

/* object api */
static void cst_component_builder_dispose(SysObject* o) {
  CstComponentBuilder *self = CST_COMPONENT_BUILDER(o);

  if(self->v_id) {
    sys_clear_pointer(&self->v_id, sys_free);
  }

  if(self->v_base_name) {
    sys_clear_pointer(&self->v_base_name, sys_free);
  }

  SYS_OBJECT_CLASS(cst_component_builder_parent_class)->dispose(o);
}

CstComponentBuilder *cst_component_builder_new(void) {
  return sys_object_new(CST_TYPE_COMPONENT_BUILDER, NULL);
}

static void cst_component_builder_construct(CstComponentBuilder *o, CstModule *v_module, CstComponent *v_parent) {
  o->v_base_name = NULL;
  o->v_parent = v_parent;
  o->v_module = v_module;
}

CstComponentBuilder *cst_component_builder_new_I(CstModule *v_module, CstComponent *v_parent) {
  CstComponentBuilder *o = cst_component_builder_new();

  cst_component_builder_construct(o, v_module, v_parent);

  return o;
}

static void cst_component_builder_class_init(CstComponentBuilderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_component_builder_dispose;
  cls->construct = cst_component_builder_construct;
}

static void cst_component_builder_init(CstComponentBuilder *self) {
}
