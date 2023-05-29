#include <TestFullText.h>

static void fulltext_basic(void) {
  char *names[10] = { "ab", "acd", "cab", "cd" };

#if 0
  [a] : [0,1,2]
  [b] : [0,2]
  [c] : [1,2,3]
  [d] : [1,3]
  [ab]: [0,2]
  [cd]: [1,3]
#endif
}

void test_fulltext_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(fulltext_basic);
  }
  UNITY_END();
}
