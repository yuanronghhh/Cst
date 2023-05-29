#include <TestCairo.h>

void test_kd_tree(void) {
  SysDouble points[10][2] = { 0 };

  srand((SysInt)time(NULL));

  for (SysUInt i = 0; i < ARRAY_SIZE(points); i++) {
    points[i][0] = sys_rand_double_range(1.0, 100.0);
    points[i][1] = sys_rand_double_range(1.0, 100.0);
  }

  SysDouble sp[][2] = {
    { 3, 2 }
  };

  SysPtrArray *npoints;
  SysKDTree *tree = sys_kdtree_new();

  sys_array_foreach(SysDouble *, item, points, ARRAY_SIZE(points)) {
    sys_kdtree_insert(tree, item);
  }

  sys_kdtree_balance(tree);
  sys_kdtree_print(tree);

  npoints = sys_kdtree_nearest(tree, sp[0], 1, 0);

  sys_array_foreach(SysKDNode *, node, npoints->pdata, npoints->len) {
    SysKDData *data = sys_kdtree_get_cords(node);

    sys_printf("%lf,%lf\n", data[0], data[1]);
  }

  sys_ptr_array_free(npoints, true);

  sys_kdtree_free(tree);
}

void cairo_basic(void) {
  fr_window_init();
  fr_canvas_init();

  FRDisplay *display = fr_display_new();
  FRWindow *window = fr_window_new(display, NULL);
  CstRender *render = cst_render_new(window);

  for (int i = 0; i < 100; i++) {
    // CstLayer *layer = cst_render_create_layer(render);
    // cst_layer_render(layer);
  }

  cst_render_flush(render);
  cst_render_unref(render);

  fr_window_unref(window);
  fr_display_unref(display);

  fr_window_deinit();
  fr_canvas_deinit();
}

void test_cairo_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_kd_tree);
    // RUN_TEST(cairo_basic);
  }
  UNITY_END();
}
