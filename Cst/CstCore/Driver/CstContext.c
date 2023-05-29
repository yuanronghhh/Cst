#include <CstCore/Driver/CstContext.h>


struct _CstContext {
  SysInt argc;
  const SysChar** argv;
  const SysChar* progname;
};

static CstContext g_context;

void cst_context_teardown(void) {
}

void cst_context_setup(SysInt argc, const SysChar* argv[]) {
  CstContext *c = &g_context;

  setlocale(LC_ALL, "");

  c->argc = argc;
  c->argv = argv;
  c->progname = NULL;
}

CstContext *cst_context_get(void) {
  return &g_context;
}

const SysChar* cst_context_progname(CstContext *c) {
  sys_return_val_if_fail(c != NULL, NULL);

  return c->progname;
}

