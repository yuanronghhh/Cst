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
  CstManager *manager;
  CstNode *root;
  CstRender *v_render;
  CstBoxLayer *box_layer;

  v_render = cst_render_new_I(USE_OFFSREEN_RENDER);
  manager = cst_manager_new_I();

  cst_manager_set_meta(manager, "layout-component", LAYOUT_TYPE_COMPONENT);

  v_module = cst_manager_load_module(manager, NULL, entry);
  TEST_ASSERT_NOT_NULL(v_module);

  cst_manager_realize(manager, v_module, v_render);
  cst_render_render(v_render);

  box_layer = cst_render_get_box_layer(v_render);
  root = cst_box_layer_get_root(box_layer);

  func(root);

  sys_object_unref(v_render);
  sys_object_unref(manager);
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
