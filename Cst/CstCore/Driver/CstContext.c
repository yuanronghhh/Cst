#include <CstCore/Driver/CstContext.h>

struct _CstContext {
  /* node parse context */
  CstModule *v_module;
  CstComponent *v_component;
  CstRender *v_render;

  /* global static data */
  SysInt argc;
  const SysChar** argv;
  const SysChar* progname;
};

void cst_context_set_v_component(CstContext *self, CstComponent * v_component) {
  sys_return_if_fail(self != NULL);

  self->v_component = v_component;
}

CstComponent * cst_context_get_v_component(CstContext *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_component;
}

void cst_context_set_v_module(CstContext *self, CstModule * v_module) {
  sys_return_if_fail(self != NULL);

  self->v_module = v_module;
}

CstModule * cst_context_get_v_module(CstContext *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_module;
}

void cst_context_set_v_render(CstContext *self, CstRender * v_render) {
  sys_return_if_fail(self != NULL);

  self->v_render = v_render;
}

CstRender * cst_context_get_v_render(CstContext *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_render;
}

CstContext *cst_context_new(void) {
  CstContext *self = sys_new0_N(CstContext, 1);
  return self;
}

void cst_context_free(CstContext* self) {
}

void cst_context_set_args(CstContext *self, int argc, char* argv[]) {
  self->argc = argc;
  self->argv = argv;
}

void cst_context_set_progname(CstContext *self, const SysChar * progname) {
  sys_return_if_fail(self != NULL);

  if(self->progname) {
    sys_clear_pointer(&self->progname, sys_free);
  }

  self->progname = sys_strdup(progname);
}

const SysChar * cst_context_get_progname(CstContext *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->progname;
}
