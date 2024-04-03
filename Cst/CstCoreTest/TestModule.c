#include "TestModule.h"

static void test_module_load(void) {
  SysChar *entry = CST_PROJECT_DIR"/Cst/CstCoreTest/Front/AppButton.cst";
  CstModule* mod;

  mod = cst_module_load_path(NULL, entry);
  TEST_ASSERT_NOT_NULL(mod);

  sys_object_unref(mod);
}

void test_module_init(int argc, SysChar* argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_module_load);
  }
  UNITY_END();
}
