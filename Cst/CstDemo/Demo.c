#include <CstDemo/TestGlib.h>
#include <CstDemo/TestGTK.h>
#include <CstDemo/Demo.h>

void setUp(void) {
}

void tearDown(void) {
}
typedef struct _FREnv FREnv;

struct _FREnv {
  SysObject parent;
  int a;
};

#define FR_TYPE_ENV (NULL)

static void callback1(char *a, char *b) {
  sys_debug_N("%s", a);
  sys_debug_N("%s", b);
}

int main(int argc, char *argv[]) {
  sys_setup();

  test_gtk_init(argc, argv);

  sys_teardown();

  return 0;
}
