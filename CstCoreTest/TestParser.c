#include <CstCoreTest/TestParser.h>


static void test_parser_basic(void) {
  SysChar *entry = CST_PROJECT_DIR"/Cst/CstCoreTest/Front/AppComponent.cst";

  CstParser *ps = cst_parser_new_I(entry);
  CstParserRContext ctx = { 0 };

  TEST_ASSERT_NOT_NULL(ps);
  TEST_ASSERT_TRUE(cst_parser_parse(ps, &ctx));

  sys_object_unref(ps);
}

void test_parser_init(int argc, SysChar* argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_parser_basic);
  }
  UNITY_END();
}
