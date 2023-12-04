#include "TestLayout.h"

#define USE_OFFSREEN_RENDER false

#define DEFINE_FLOW_FUNC(func_name, path) \
static void _##func_name(void) { \
  SysChar *entry = path; \
  test_layout_box_workflow(entry, func_name); \
}

typedef void (*TestLayoutFunc) (CstNode *tree);

static void test_layout_box_workflow(SysChar *entry, TestLayoutFunc func) {
  CstModule* v_module;
  CstNode *root;
  CstRender *v_render;

  v_render = cst_render_new_I(USE_OFFSREEN_RENDER);

  cst_render_node_set_meta("layout-component", LAYOUT_TYPE_COMPONENT);

  v_module = cst_module_load_path(NULL, entry);
  TEST_ASSERT_NOT_NULL(v_module);

  cst_render_render(v_render, v_module);

  root = cst_node_get_body_node();

  func(root);

  sys_object_unref(v_render);
}

static void test_layout_wrap(CstNode *tree) {
  getchar();
}

static void test_layout_component_basic (CstNode *tree) {
}

static void test_layout_row (CstNode *tree) {
  getchar();
}

DEFINE_FLOW_FUNC(test_layout_wrap, CST_PROJECT_DIR"/Cst/CstCoreTest/Front/LayoutWrap.cst");
// DEFINE_FLOW_FUNC(test_layout_row, CST_PROJECT_DIR"/Cst/CstCoreTest/Front/LayoutRow.cst")
// DEFINE_FLOW_FUNC(test_layout_component_basic, CST_PROJECT_DIR"/Cst/CstCoreTest/Front/LayoutComponentBasic.cst")

void test_layout_init(int argc, SysChar * argv[]) {

  UNITY_BEGIN();
  {
    RUN_TEST(_test_layout_wrap);
  }
  UNITY_END();
}
