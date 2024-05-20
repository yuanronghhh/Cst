#include <Framework/ThirdParty/FrFont.h>

static PangoFontMap *font_map = NULL;

void fr_font_setup(void) {
  sys_debug_N("%s", "init pango font map, maybe slow on win32 ...");

  font_map = pango_cairo_font_map_get_default();
}

void fr_font_teardown(void) {
  sys_assert(font_map != NULL && "fr_font must be inited");

  g_object_unref(font_map);
}
