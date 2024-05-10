#include <Framework/FrCore.h>

static SysBool inited = false;

void fr_core_setup(void) {
  if (inited) { return; }

  sys_setup();
  fr_ffmpeg_setup();
  fr_main_setup();
  fr_window_setup();

#if defined(CAIRO_VERSION)
  fr_draw_setup("cairo");
#endif

  fr_events_setup();

#if defined(MONO_CORLIB_VERSION)
  fr_mono_setup(FR_MONO_HOME);
#endif

  inited = true;
}

void fr_core_teardown(void) {
  if(!inited) { return; }

#if defined(CAIRO_VERSION)
  fr_draw_teardown();
#endif

#if defined(MONO_CORLIB_VERSION)
  fr_mono_setup(FR_MONO_HOME);
#endif

  fr_events_teardown();
  fr_window_teardown();
  fr_main_teardown();

  fr_ffmpeg_teardown();
  sys_teardown();

  inited = false;
}
