#include <Mini/Mini.h>

static void object_ref_debug(SysObject *o, const SysChar *name, SysInt ref_count) {
  if (!sys_object_is_a(o, FR_TYPE_MAIN)) {
    return;
  }

  sys_debug_N("%p\t%d", o, ref_count);
  sys_assert(ref_count > 0);
}

static void object_unref_debug(SysObject *o, const SysChar *name, SysInt ref_count) {
  if (!sys_object_is_a(o, FR_TYPE_MAIN)) {
    return;
  }

  sys_debug_N("%p\t%d", o, ref_count);
  sys_assert(ref_count > 0);
}

int main(int argc, char* argv[]) {
  int status = 0;
  CstApplication *app;

  sys_setup();

  sys_object_set_ref_hook(object_ref_debug);
  sys_object_set_unref_hook(object_unref_debug);

  const SysChar *main_path = CST_PROJECT_DIR"/Cst/Mini/Front/MiniComponent.cst";

  app = cst_application_new_I("MiniAPP");

  mini_types_setup(app);

  status = cst_application_run(app, main_path);
  sys_object_unref(app);

  sys_teardown();

  return status;
}
