#include "CstCoreTest.h"

void setUp(void) {
}

void tearDown(void) {
}

void cst_test_init(CstInt argc, CstChar* argv[]) {
  cst_context_init(argc, argv);
}

void cst_test_finalize() {
  cst_context_destroy();
}

int main(int argc, char* argv[]) {
  cst_test_init(argc, argv);

  cst_test_finalize();

  printf("[Done]\n");
#if _MSC_VER
  getchar();
#endif

  return 0;
}
