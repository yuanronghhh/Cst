#include <TestInApplication.h>

static void app_activate() {
  g_print("activated\n");
}

static void test_application() {
  CstApplication *app;
  int status;

  app = cst_application_new("Org.Cst.HelloWorld", G_APPLICATION_HANDLES_OPEN);
  status = g_application_run(G_APPLICATION(app), 0, NULL);

  g_object_unref(app);
}

void test_application_init(void) {
  UNITY_BEGIN();
  {
    // RUN_TEST(test_gtk_run);
    RUN_TEST(test_application);
  }
  UNITY_END();
}
