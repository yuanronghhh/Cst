#include <TestOOC.h>

#if !defined(MANY) || (MANY) < 1
#define MANY 10
#endif


/* ---------- test ---------- */
static void ooc_basic(void) {
}

void test_ooc_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(ooc_basic);
  }
  UNITY_END();
}
