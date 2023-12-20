#include <Framework/FRCore.h>

static SysBool inited = false;

void fr_core_setup(void) {
  if (inited) { return; }

  sys_setup();
  fr_main_setup();
  fr_window_setup();
  fr_canvas_setup();
  fr_events_setup();
#if CST_USE_MONO
  fr_mono_setup(CST_MONO_HOME);
#endif

  inited = true;
}

void fr_core_teardown(void) {
  if(!inited) { return; }

  fr_events_teardown();
  fr_canvas_teardown();
  fr_window_teardown();
  fr_main_teardown();
  sys_teardown();

  inited = false;
}
