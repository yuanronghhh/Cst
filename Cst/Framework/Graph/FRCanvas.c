#include <Framework/Graph/FRCanvasPrivate.h>
#include <Framework/Device/FRDisplay.h>
#include <Framework/Device/FRWindow.h>
#include <Framework/Graph/FRText.h>


struct _FRDraw {
  SysPointer v;

  /* not support double buffer now */
  FRSurface *surface;
  FRDisplay *display;
  FRWindow *window;
};


SysInt fr_canvas_rounded_rectangle(FRContext* context, SysDouble x, SysDouble y, SysDouble w, SysDouble h, SysDouble radius) {
  cairo_t *cr = (cairo_t *)context;

  SysDouble degrees = M_PI / 180.0;

  cairo_new_sub_path(cr);
  cairo_arc(cr, x + w - radius, y + radius, radius, -90 * degrees, 0 * degrees);
  cairo_arc(cr, x + w - radius, y + h - radius, radius, 0 * degrees, 90 * degrees);
  cairo_arc(cr, x + radius, y + h - radius, radius, 90 * degrees, 180 * degrees);
  cairo_arc(cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
  cairo_close_path(cr);

  cairo_set_source_rgb(cr, 0.5, 0.5, 1);
  cairo_fill_preserve(cr);
  cairo_set_source_rgba(cr, 0.5, 0, 0, 0.5);
  cairo_set_line_width(cr, 10.0);
  cairo_stroke(cr);

  return cairo_status(cr);
}

void fr_canvas_setup(void) {
  fr_font_setup();
}

void fr_canvas_teardown(void) {
  fr_font_teardown();
}
