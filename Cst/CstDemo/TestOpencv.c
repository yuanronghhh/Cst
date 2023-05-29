#include <TestOpencv.h>

static void test_basic(void) {
}

void test_opencv_init(void) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_basic);
  }
  UNITY_END();
}
