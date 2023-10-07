#include <Framework/ThirdParty/FRCairo.h>
#include <Framework/ThirdParty/FRFont.h>


static void draw_text(cairo_t* cr, const SysChar *text) {
#define RADIUS 150
#define N_WORDS 2
#define FONT "Sans Bold 27"

  PangoLayout *layout;
  PangoFontDescription *desc;
  int i;

  cairo_translate(cr, RADIUS, RADIUS);

  layout = pango_cairo_create_layout(cr);

  pango_layout_set_text(layout, text, -1);
  desc = pango_font_description_from_string(FONT);
  pango_layout_set_font_description(layout, desc);
  pango_font_description_free(desc);

  for (i = 0; i < N_WORDS; i++)
  {
    int width, height;
    double angle = (360. * i) / N_WORDS;
    double red;

    cairo_save(cr);

    red = (1 + cos((angle - 60) * G_PI / 180.)) / 2;
    cairo_set_source_rgb(cr, red, 0, 1.0 - red);

    cairo_rotate(cr, angle * G_PI / 180.);

    pango_cairo_update_layout(cr, layout);

    pango_layout_get_size(layout, &width, &height);
    cairo_move_to(cr, -((double)width / PANGO_SCALE) / 2, -RADIUS);
    pango_cairo_show_layout(cr, layout);

    cairo_restore(cr);
  }

  g_object_unref(layout);
}

void fr_cairo_demo(cairo_t* cr) {
  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  cairo_paint(cr);
  draw_text(cr, "hello");
}