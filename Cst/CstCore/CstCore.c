#include <CstCore/CstCore.h>


static SysBool inited = false;


void cst_core_setup(void) {
  if(inited) { return; }

  setlocale(LC_ALL, "");

  sys_setup();
  fr_main_setup();
  fr_window_setup();
  fr_canvas_setup();
  fr_events_setup();
#if CST_USE_MONO
  fr_mono_setup(CST_MONO_HOME);
#endif

  cst_context_setup();
  cst_css_setup();
  cst_node_setup();
  cst_render_node_setup();
  cst_module_setup();

  inited = true;
}

void cst_core_teardown(void) {
  if(!inited) { return; }

  cst_context_teardown();
  cst_render_node_teardown();
  cst_node_teardown();
  cst_module_teardown();
  cst_css_teardown();
  fr_events_teardown();
  fr_canvas_teardown();
  fr_window_teardown();
  fr_main_teardown();
  sys_teardown();
}

