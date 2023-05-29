#include <Framework/Graph/FRCanvasPrivate.h>
#include <Framework/Device/FRDisplay.h>
#include <Framework/Device/FRWindow.h>
#include <Framework/Graph/FRText.h>


void fr_canvas_setup(void) {
}

void fr_canvas_teardown(void) {
}

/* FRContext */
FRContext* fr_draw_create_context(FRDraw* draw) {
  sys_return_val_if_fail(draw != NULL, NULL);

  FRContext *cr;

  sys_assert(draw->surface != NULL && "draw->surface should set before create context.");

  cr = cairo_create(draw->surface);

  return cr;
}

FRContext* fr_draw_get_cr(FRDraw* draw) {
  sys_return_val_if_fail(draw != NULL, NULL);

  return draw->cr;
}

/* FRDraw */
void fr_draw_frame_begin(FRDraw *self, FRRegion *region) {
  cairo_t *cr;
  int n_boxes, i;
  cairo_rectangle_int_t box;

  cr = self->cr;

  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0);
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

  n_boxes = cairo_region_num_rectangles(region);

  for (i = 0; i < n_boxes; i++) {
    cairo_region_get_rectangle(region, i, &box);
    cairo_rectangle(cr, box.x, box.y, box.width, box.height);
  }

  cairo_clip(cr);
  cairo_paint(cr);
}

void fr_draw_frame_end(FRDraw *self, FRRegion *region) {
  cairo_surface_flush(self->surface);
}

FRDraw* fr_canvas_create_draw(FRWindow *window) {
  FRDraw* draw = sys_new0_N(FRDraw, 1);

  draw->window = window;

  if (window) {
    draw->display = fr_window_get_display(window);
  }

  draw->surface = fr_draw_create_surface(draw);
  draw->cr = fr_draw_create_context(draw);

  return draw;
}

void fr_draw_destroy(FRDraw* draw) {
  sys_return_if_fail(draw != NULL);

  sys_clear_pointer(&draw->cr, cairo_destroy);
  sys_clear_pointer(&draw->surface, cairo_surface_destroy);

  sys_free_N(draw);
}

/* FRSurface */
FRSurface *fr_draw_get_surface(FRDraw *draw) {
  return draw->surface;
}

FRSurface* fr_draw_create_surface(FRDraw* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  SysInt fbw = 0, fbh = 0;
  FRSurface *surface;

  if (self->window == NULL) {
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 600);
    return surface;
  }

#if SYS_OS_WIN32
  HWND hwd = fr_window_get_win32_window(self->window);
  HDC hdc = GetDC(hwd);
  surface = cairo_win32_surface_create(hdc);
#elif SYS_OS_UNIX
  Window xwindow = fr_window_get_x11_window(self->window);
  Display *ndisplay = fr_display_get_x11_display(self->display);

  fr_window_get_framebuffer_size(self->window, &fbw, &fbh);

  surface = cairo_xlib_surface_create(ndisplay, xwindow, DefaultVisual(ndisplay, DefaultScreen(ndisplay)), fbw, fbh);
#endif

  return surface;
}

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
