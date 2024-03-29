#include <CstCore/CstCore.h>


static SysBool inited = false;


void cst_core_setup(void) {
  if(inited) { return; }

  setlocale(LC_ALL, "");

  sys_setup();
  fr_core_setup();
  cst_context_setup();
  cst_css_setup();
  cst_render_setup();
  cst_node_setup();
  cst_component_setup();
  cst_render_node_setup();
  cst_module_setup();

  inited = true;
}

void cst_core_teardown(void) {
  if(!inited) { return; }

  cst_context_teardown();
  cst_render_node_teardown();
  cst_component_teardown();
  cst_render_teardown();
  cst_node_teardown();
  cst_module_teardown();
  cst_css_teardown();
  fr_core_teardown();
  sys_teardown();
}

