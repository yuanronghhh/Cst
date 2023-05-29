#include <TestBDWGC.h>

static void test_basic(void) {
  int i;
  GC_stop_func stop;

  GC_INIT();
  for (i = 0; i < 10000000; ++i) {
    int **p = (int **) GC_MALLOC(sizeof(int *));
    int *q = (int *) GC_MALLOC_ATOMIC(sizeof(int));
    sys_assert(*p == 0);

    *p = (int *) GC_REALLOC(q, 2 * sizeof(int));
    if (i % 100000 == 0)
      printf("Heap size = %lu bytes\n",
          (unsigned long)GC_get_heap_size());
  }

  stop = GC_get_stop_func();
  GC_try_to_collect(stop);
}

void test_bdwgc_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_basic);
  }
  UNITY_END();
  getchar();
}
