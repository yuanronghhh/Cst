#include <Framework/ThirdParty/FrFont.h>


void fr_font_setup(void) {
  sys_debug_N("%s", "init pango font map, maybe slow ...");

  SYS_LEAK_IGNORE_BEGIN;
  pango_cairo_font_map_get_default();
  SYS_LEAK_IGNORE_END;
}

void fr_font_teardown(void) {
}
