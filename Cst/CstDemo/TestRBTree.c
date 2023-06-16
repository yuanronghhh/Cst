#include <CstDemo/TestRBTree.h>


static void test_rbtree_basic(void) {
}

void test_rbtree_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_rbtree_basic);
  }
  UNITY_END();
}
