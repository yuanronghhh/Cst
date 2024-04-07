#include <Framework/Graph/FrCairoDraw.h>
#include <Framework/Device/FrWindow.h>
#include <Framework/Device/FrDisplay.h>
#include <Framework/Device/FrWindow.h>


static void i_draw_imp(FrIDrawInterface *iface);

SYS_DEFINE_WITH_CODE(FrCairoDraw, fr_cairo_draw, FR_TYPE_DRAW,
    SYS_IMPLEMENT_INTERFACE(FR_TYPE_I_DRAW, i_draw_imp));

void cairo_stroke_mp_i(FrDrawBrush* cr, const FrBound *bound, const FrSInt4* m4, const FrSInt4* p4) {
  sys_return_if_fail(cr != NULL);
  sys_return_if_fail(m4 != NULL);
  sys_return_if_fail(p4 != NULL);
  sys_return_if_fail(bound != NULL);

  SysInt x = bound->x + m4->m3;
  SysInt y = bound->y + m4->m0;
  SysInt width = bound->width + p4->m1 + p4->m3;
  SysInt height = bound->height + p4->m0 + p4->m2;

  cairo_rectangle(cr, x, y, width, height);
  cairo_stroke(cr);
}

void cairo_context_fill_bound_i(FrDrawBrush* cr, const FrBound *bound) {
  sys_return_if_fail(cr != NULL);
  sys_return_if_fail(bound != NULL);

  cairo_rectangle(cr, bound->x, bound->y, bound->width, bound->height);
  cairo_fill(cr);
}

/* surface */
FrDrawSurface* cairo_image_surface_create_i(SysInt width, SysInt height) {
  return cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
}

FrDrawSurface* cairo_create_surface_i(FrIDevice* device, SysInt width, SysInt height) {
  sys_return_val_if_fail(device != NULL, NULL);

  FrDrawSurface* surface;

  /* only support window now */
  FrWindow *window = FR_WINDOW(device);

#if SYS_OS_WIN32
  HWND hwd = fr_window_get_win32_window(window);
  HDC hdc = GetDC(hwd);
  surface = cairo_win32_surface_create_with_format(hdc, CAIRO_FORMAT_ARGB32);

#elif SYS_OS_UNIX
  FrDisplay* display = fr_window_get_display(window);
  Window xwindow = fr_window_get_x11_window(window);
  Display* ndisplay = fr_display_get_x11_display(display);
  int nscreen = DefaultScreen(ndisplay);
  Visual* nvisual = DefaultVisual(ndisplay, nscreen);

  surface = cairo_xlib_surface_create(ndisplay,
    xwindow,
    nvisual,
    width, height);
#endif

  cairo_t* cr = cairo_create(surface);
  cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
  cairo_paint(cr);
  cairo_destroy(cr);

  return surface;
}

FrDrawSurface* cairo_surface_create_similar_image_i(FrDrawSurface *surface, SysInt width, SysInt height) {
  FrDrawSurface * nsur = cairo_surface_create_similar_image(surface,
      CAIRO_FORMAT_ARGB32,
      width,
      height);

  return nsur;
}

void cairo_context_fill_background_i(FrDrawBrush *cr, SysInt width, SysInt height) {
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.5);
  cairo_rectangle(cr, 0, 0, width, height);
  cairo_paint(cr);
}

SysInt cairo_rounded_rectangle_i(FrDrawBrush* cr,
  SysDouble x, SysDouble y, SysDouble w, SysDouble h, 
  SysDouble radius) {

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

static void cairo_overlay_i(FrDrawBrush* cr, FrDrawSurface *surface, SysInt x, SysInt y) {

  cairo_set_source_surface(cr, surface, x, y);
  cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
}

static void cairo_print_text_i (FrDrawBrush* cr, SysDouble x,SysDouble y, const SysChar *text) {
  cairo_move_to(cr, x, y);
  cairo_show_text(cr, text);
}

static void i_draw_imp(FrIDrawInterface *iface) {
  iface->overlay = cairo_overlay_i;
  iface->set_source_rgba = cairo_set_source_rgba;
  iface->set_source_surface = cairo_set_source_surface;
  iface->rectangle = cairo_rectangle;
  iface->paint = cairo_paint;
  iface->create = cairo_create;
  iface->stroke = cairo_stroke;
  iface->clip = cairo_clip;
  iface->fill = cairo_fill;
  iface->destroy = cairo_destroy;
  iface->rounded_rectangle = cairo_rounded_rectangle_i;
  iface->image_surface_create = cairo_image_surface_create_i;
  iface->create_surface = cairo_create_surface_i;
  iface->surface_create_similar_image = cairo_surface_create_similar_image_i;
  iface->surface_flush = cairo_surface_flush;
  iface->surface_destroy = cairo_surface_destroy;
  iface->print_text = cairo_print_text_i;
}

/* object api */
static void fr_cairo_draw_construct(FrDraw *o) {

  FR_DRAW_CLASS(fr_cairo_draw_parent_class)->construct(o);
}

FrDraw* fr_cairo_draw_new(void) {
  return sys_object_new(FR_TYPE_CAIRO_DRAW, NULL);
}

FrDraw *fr_cairo_draw_new_I(void) {
  FrDraw *o = fr_cairo_draw_new();

  fr_cairo_draw_construct(o);

  return o;
}

static void fr_cairo_draw_dispose(SysObject* o) {
  // FrCairoDraw *self = FR_CAIRO_DRAW(o);

  SYS_OBJECT_CLASS(fr_cairo_draw_parent_class)->dispose(o);
}

static void fr_cairo_draw_class_init(FrCairoDrawClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrDrawClass* dcls = FR_DRAW_CLASS(cls);

  dcls->construct = fr_cairo_draw_construct;
  ocls->dispose = fr_cairo_draw_dispose;
}

void fr_cairo_draw_init(FrCairoDraw* self) {
}
