#include "TestWindow.h"

static void test_window_basic(void) {
  CstRender *render;

  render = cst_render_new_I(false);
  sys_object_unref(render);
}

static void test_pango_leak(void) {
  PangoFontMap *font_map = pango_cairo_font_map_get_default();
  PangoContext *pctx = pango_font_map_create_context(font_map);

  PangoLayout *playout = pango_layout_new (pctx);
  g_object_unref(pctx);
  g_object_unref(playout);
}

void test_window_init(int argc, SysChar* argv[]) {
  UNITY_BEGIN();
  {
    // RUN_TEST(test_window_basic);
    RUN_TEST(test_pango_leak);
  }
  UNITY_END();
}
