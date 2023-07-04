#include <CstDemo/TestSTD.h>
#include <CstDemo/Demo.h>

void setUp(void) {
  unsigned short a = ~0xff38 + 1;

  printf("%d", a);
}

void tearDown(void) {
}

int main(int argc, char *argv[]) {
  sys_setup();

  test_std_init(argc, argv);

  sys_teardown();

  return 0;
}
