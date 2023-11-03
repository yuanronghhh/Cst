#include <CstCore/CstCore.h>

#include <CstCore/Driver/Css/CstCss.h>


void cst_core_setup(void) {
  setlocale(LC_ALL, "");

  fr_main_setup();
  fr_window_setup();
  fr_canvas_setup();
  fr_events_setup();
#if CST_USE_MONO
  fr_mono_setup(CST_MONO_HOME);
#endif

  cst_css_setup();
  cst_module_setup();
  cst_css_node_setup();
}

void cst_core_teardown(void) {
  cst_render_node_teardown();
  cst_module_setup();
  cst_css_teardown();
  fr_events_teardown();
  fr_canvas_teardown();
  fr_window_teardown();
  fr_main_teardown();
}

