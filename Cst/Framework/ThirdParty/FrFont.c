#include <Framework/ThirdParty/FrFont.h>

static PangoFontMap *font_map = NULL;

void fr_font_setup(void) {
  sys_debug_N("%s", "init pango font map, maybe slow ...");

  SYS_LEAK_IGNORE_BEGIN;
  font_map = pango_cairo_font_map_get_default();
  SYS_LEAK_IGNORE_END;
}

void fr_font_teardown(void) {
  sys_assert(font_map != NULL && "fr_font must be inited");

  g_object_unref(font_map);
}
