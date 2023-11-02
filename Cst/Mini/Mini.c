#include <Mini/Mini.h>

#define CHECK_TYPE FR_TYPE_AWATCH

static void object_new_debug(SysObject *o, const SysChar *name, SysInt ref_count) {
  if (!sys_object_is_a(o, CHECK_TYPE)) {
    return;
  }

  sys_debug_N("%p\t%s\t%d", o, name, ref_count);
}

static void object_ref_debug(SysObject *o, const SysChar *name, SysInt ref_count) {
  if (!sys_object_is_a(o, CHECK_TYPE)) {
    return;
  }

  sys_debug_N("%p\t%s\t%d", o, name, ref_count);
  sys_assert(ref_count > 0);
}

static void object_unref_debug(SysObject *o, const SysChar *name, SysInt ref_count) {
  if (!sys_object_is_a(o, CHECK_TYPE)) {
    return;
  }

  sys_debug_N("%p\t%s\t%d", o, name, ref_count);
  sys_assert(ref_count > 0);
}

int main(int argc, char* argv[]) {
  int status;
  CstApplication *app;
  CstContext *c;
  const SysChar *main_path;

  sys_setup();

  sys_object_set_ref_hook(object_ref_debug);
  sys_object_set_unref_hook(object_unref_debug);
  sys_object_set_new_hook(object_new_debug);

  status = 0;
  main_path = CST_PROJECT_DIR"/Cst/Mini/Front/MiniComponent.cst";
  c = cst_context_new();

  cst_context_set_args(argc, argv);
  mini_types_setup();

  app = cst_application_new_I(c, "MiniAPP");
  status = cst_application_run(app, main_path);
  sys_object_unref(app);

  sys_teardown();

  return status;
}
