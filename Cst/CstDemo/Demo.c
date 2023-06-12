#include <CstDemo/TestGlib.h>
#include <CstDemo/TestGTK.h>
#include <CstDemo/TestRBTree.h>
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

int main(int argc, char *argv[]) {
  sys_setup();

  test_rbtree_init(argc, argv);

  sys_teardown();

  return 0;
}
