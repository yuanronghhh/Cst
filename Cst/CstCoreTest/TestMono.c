#include "TestMono.h"

#if CST_USE_MONO
static void _test_mono_basic(void) {
  CstComponent *component = cst_component_new();
  CstText *text = cst_text_new();
}

void test_mono_init(int argc, SysChar * argv[]) {

  UNITY_BEGIN();
  {
    RUN_TEST(_test_mono_basic);
  }
  UNITY_END();
}
#endif
