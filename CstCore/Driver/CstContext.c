#include <CstCore/Driver/CstContext.h>

struct _CstContext {
  /* global static data */
  SysInt argc;
  SysChar** argv;
  SysChar* progname;
};

static CstContext g_context = {0};

void cst_context_set_args(int argc, char* argv[]) {
  g_context.argc = argc;
  g_context.argv = argv;
}

void cst_context_setup (void) {
}

void cst_context_teardown (void) {
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
