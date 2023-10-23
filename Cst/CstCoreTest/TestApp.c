#include "TestApp.h"

static void app_types_setup(void) {
  cst_node_set_meta("app-button", APP_TYPE_BUTTON);
  cst_node_set_meta("app-component", APP_TYPE_COMPONENT);
}

static void test_app_basic(void) {
  int status;
  CstApplication *app;
  const SysChar *entry = CST_PROJECT_DIR"/Cst/CstCoreTest/Front/AppComponent.cst";

  app = cst_application_new_I("TestAPP");
  TEST_ASSERT_NOT_NULL(app);

  app_types_setup();

  status = cst_application_run(app, entry);
  sys_object_unref(app);

  UNUSED(status);
}

void test_app_init(int argc, SysChar* argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_app_basic);
  }
  UNITY_END();
}
