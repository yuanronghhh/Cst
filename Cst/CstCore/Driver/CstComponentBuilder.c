#include <CstCore/Driver/CstComponentBuilder.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstComponent.h>


SYS_DEFINE_TYPE(CstComponentBuilder, cst_component_builder, SYS_TYPE_OBJECT);


void cst_component_builder_build(CstComponentBuilder *self, CstComponent *v_component) {
}

void cst_component_builder_set_base_name(CstComponentBuilder *self, SysChar *v_base_name) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_base_name != NULL);

  self->v_base_name = sys_strdup(v_base_name);
}

void cst_component_builder_set_id(CstComponentBuilder *self, const SysChar *v_id) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_id != NULL);

  self->v_id = sys_strdup(v_id);
}

const SysChar *cst_component_builder_get_id(CstComponentBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_id;
}

/* object api */
static void cst_component_builder_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(cst_component_builder_parent_class)->dispose(o);
}

CstComponentBuilder *cst_component_builder_new(void) {
  return sys_object_new(CST_TYPE_COMPONENT_BUILDER, NULL);
}

void cst_component_builder_construct(CstComponentBuilder *o, const SysChar *v_id) {
  o->v_id = sys_strdup(v_id);
  o->v_base_name = NULL;
}

CstComponentBuilder *cst_component_builder_new_I(const SysChar *v_id) {
  CstComponentBuilder *o = cst_component_builder_new();

  cst_component_builder_construct(o, v_id);

  return o;
}

static void cst_component_builder_class_init(CstComponentBuilderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_component_builder_dispose;
  cls->construct = cst_component_builder_construct;
}

static void cst_component_builder_init(CstComponentBuilder *self) {
}
