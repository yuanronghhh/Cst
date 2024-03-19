#include <Framework/ThirdParty/FRFont.h>


void fr_font_setup(void) {
  SYS_LEAK_IGNORE_BEGIN;
  pango_cairo_font_map_get_default();
  SYS_LEAK_IGNORE_END;
}

void fr_font_teardown(void) {
}
