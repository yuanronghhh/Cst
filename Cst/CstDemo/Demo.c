#include <CstDemo/TestGLFW.h>
#include <CstDemo/TestGTK.h>
#include <CstDemo/Demo.h>

void setUp(void) {
}

void tearDown(void) {
}

int main(int argc, char *argv[]) {
  sys_setup();

  test_gtk_init(argc, argv);

  sys_teardown();

  return 0;
}
