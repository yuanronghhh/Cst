#include "TestCore.h"

void setUp(void) {
}

void tearDown(void) {
}

int main(int argc, SysChar* argv[]) {
  sys_setup();

  cst_core_setup();

  test_app_init(argc, argv);
  // test_fr_init(argc, argv);
  // test_window_init(argc, argv);
  // test_module_init(argc, argv);
  // test_parser_init(argc, argv);
  // test_layout_init(argc, argv);
#if CST_USE_MONO
  test_mono_init(argc, argv);
#endif

  cst_core_teardown();

  sys_teardown();
  return 0;
}
