#include <TestGC.h>

static void gc_basic(void) {
}

void test_gc_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(gc_basic);
  }
  UNITY_END();
}
