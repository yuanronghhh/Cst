#include "TestFramework.h"

static const SysChar *FR_ACTION_NAMES[] = {
  "key_down",
  "key_up",
  "drag_start",
  "drag_end",
};

void test_fr_basic(void) {
  SysInt ftype = fr_get_type_by_name((const SysChar **)FR_ACTION_NAMES, ARRAY_SIZE(FR_ACTION_NAMES), "key_up");

  UNUSED(ftype);
}

void test_fr_window_basic(void) {
  FrDisplay *display = fr_display_new_I();
  FrWindow *window = fr_window_top_new(display);

  sys_object_unref(window);
  sys_object_unref(display);
}

void test_fr_window_leak(void) {
  GLFWwindow *gwindow;

  gwindow = glfwCreateWindow(800, 600, "Leak Demo", NULL, NULL);

  glfwDestroyWindow(gwindow);
  glfwTerminate();
}

void test_fr_init(int argc, SysChar * argv[]) {
  UNITY_BEGIN();
  {
    // RUN_TEST(test_fr_basic);
    RUN_TEST(test_fr_window_leak);
    // RUN_TEST(test_fr_window_basic);
  }
  UNITY_END();
}
