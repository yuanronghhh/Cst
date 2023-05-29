#include "TestWindow.h"

static void test_window_basic(void) {
  CstRender *render;

  render = cst_render_new_I(false);
  sys_object_unref(render);
}

void test_window_init(int argc, SysChar* argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_window_basic);
  }
  UNITY_END();
}
