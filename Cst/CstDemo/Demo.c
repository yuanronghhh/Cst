#include <CstDemo/TestFRP.h>
#include <CstDemo/Demo.h>

void setUp(void) {
}

void tearDown(void) {
}

int main(int argc, const char *argv[]) {
  sys_setup();

  test_frp_init(argc, argv);

  sys_teardown();

  return 0;
}
