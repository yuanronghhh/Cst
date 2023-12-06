#include "TestBasic.h"

static void test_leak(void) {
  CstModule* v_module;
  SysChar *entry = CST_PROJECT_DIR"/Cst/CstCoreTest/Front/AppBox.cst";

  cst_render_node_set_meta("app-box", APP_TYPE_BOX);

  v_module = cst_module_load_path(NULL, entry);
  TEST_ASSERT_NOT_NULL(v_module);
}

void test_basic_init(int argc, SysChar * argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_leak);
  }
  UNITY_END();
}
