#include "TestCore.h"

#define CHECK_TYPE CST_TYPE_NODE


void setUp(void) {
}

void tearDown(void) {
}

static void object_new_debug(SysObject *o, const SysChar *name, SysInt ref_count) {
  if (!sys_object_is_a(o, CHECK_TYPE)) {
    return;
  }

  sys_debug_N("%p\t%s\t%d", o, name, ref_count);
  sys_assert(ref_count == 1);
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

int main(int argc, SysChar* argv[]) {
  sys_setup();

  sys_object_set_ref_hook(object_ref_debug);
  sys_object_set_unref_hook(object_unref_debug);
  sys_object_set_new_hook(object_new_debug);

  cst_core_setup();

#if CST_USE_MONO
  test_mono_init(argc, argv);
#endif

  // test_basic_init(argc, argv);
  test_app_init(argc, argv);
  // test_fr_init(argc, argv);
  // test_window_init(argc, argv);
  // test_module_init(argc, argv);
  // test_parser_init(argc, argv);
  // test_layout_init(argc, argv);

  cst_core_teardown();

  sys_teardown();
  return 0;
}
