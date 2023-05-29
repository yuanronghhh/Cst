#include "TestModule.h"

static void test_module_load(void) {
  SysChar *entry = CST_PROJECT_DIR"/Cst/CstCoreTest/Front/AppButton.cst";
  CstModule* mod;

  CstManager *manager = cst_manager_new();

  cst_manager_lock(manager);

  mod = cst_manager_load_module(manager, NULL, entry);
  TEST_ASSERT_NOT_NULL(mod);

  cst_manager_unlock(manager);

  sys_object_unref(manager);
}

void test_module_init(int argc, SysChar* argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_module_load);
  }
  UNITY_END();
}
