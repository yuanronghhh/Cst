#include <CstDemo/TestCairo.h>
#include <CstDemo/Demo.h>

void setUp(void) {
}

void tearDown(void) {
}

int main(int argc, char *argv[]) {
  sys_setup();

  test_cairo_init(argc, argv);

  sys_teardown();

  return 0;
}
