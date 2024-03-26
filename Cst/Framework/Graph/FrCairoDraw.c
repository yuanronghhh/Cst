#include <Framework/Graph/FrCairoDraw.h>

#include <Framework/Device/FrWindow.h>
#include <Framework/Graph/FrIDraw.h>
#include <Framework/Device/FrDisplay.h>
#include <Framework/Device/FrWindow.h>


static void i_draw_imp(FrIDrawInterface *iface);

SYS_DEFINE_WITH_CODE(FrCairoDraw, fr_cairo_draw, SYS_TYPE_OBJECT,
    SYS_IMPLEMENT_INTERFACE(FR_TYPE_I_DRAW, i_draw_imp));

void cairo_stroke_mp_i(FrDrawContext* cr, const FrRect *bound, const FrSInt4* m4, const FrSInt4* p4) {
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

void cairo_context_fill_bound_i(FrDrawContext* cr, const FrRect *bound) {
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

  cairo_t* cr = cairo_create(surface);
  cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
  cairo_paint(cr);
  cairo_destroy(cr);

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

  return surface;
}

FrDrawSurface* cairo_surface_create_similar_image_i(FrDrawSurface *surface, SysInt width, SysInt height) {
  FrDrawSurface * nsur = cairo_surface_create_similar_image(surface,
      CAIRO_FORMAT_ARGB32,
      width,
      height);

  return nsur;
}

void cairo_context_fill_background_i(FrDrawContext *cr, SysInt width, SysInt height) {
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.5);
  cairo_rectangle(cr, 0, 0, width, height);
  cairo_paint(cr);
}

static void i_draw_imp(FrIDrawInterface *iface) {
  iface->image_surface_create = cairo_image_surface_create_i;
  iface->create_surface = cairo_create_surface_i;
  iface->surface_create_similar_image = cairo_surface_create_similar_image_i;
}

/* object api */
static void fr_cairo_draw_construct(FrDraw *o, FrIDevice *device) {

  FR_DRAW_CLASS(fr_cairo_draw_parent_class)->construct(o, device);
}

FrDraw* fr_cairo_draw_new(void) {
  return sys_object_new(FR_TYPE_CAIRO_DRAW, NULL);
}

FrDraw *fr_cairo_draw_new_I(FrIDevice *device) {
  FrDraw *o = fr_cairo_draw_new();

  fr_cairo_draw_construct(o, device);

  return o;
}

static void fr_cairo_draw_dispose(SysObject* o) {
  FrCairoDraw *self = FR_CAIRO_DRAW(o);

  SYS_OBJECT_CLASS(fr_cairo_draw_parent_class)->dispose(o);
}

static void fr_cairo_draw_class_init(FrCairoDrawClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_cairo_draw_dispose;
}

void fr_cairo_draw_init(FrCairoDraw* self) {
}
