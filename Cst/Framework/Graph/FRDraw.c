#include <Framework/Graph/FRDraw.h>
#include <Framework/Device/FRDisplay.h>
#include <Framework/Device/FRWindow.h>


struct _FRDrawPrivate {
  SysPointer v;

  SysInt64 last_clock;
  SysDouble rate_time;

  /* not support double buffer now */
  FRSurface *window_surface;
  FRSurface *paint_surface;
  FRDisplay *display;
  FRWindow *window;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRDraw, fr_draw, SYS_TYPE_OBJECT);

SysBool fr_draw_frame_need_draw(FRDraw *self) {
  sys_return_val_if_fail(self != NULL, false);

  FRDrawPrivate *priv = self->priv;
  SysInt64 current = sys_get_monotonic_time();

  if (priv->last_clock == 0) {
    priv->last_clock = current;
    return true;
  }

  SysBool r = (current - priv->last_clock) > priv->rate_time;
  priv->last_clock = r ? current : priv->last_clock;

  return r;
}

/* FRDraw */

void fr_draw_get_size(FRDraw *self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);
  FRDrawPrivate *priv = self->priv;

  fr_window_get_size(priv->window, width, height);
}

FRContext* fr_draw_create_cr(FRDraw* self) {
  sys_return_val_if_fail(self != NULL, NULL);
  FRContext *cr;

  FRDrawPrivate *priv = self->priv;

  sys_assert(priv->paint_surface != NULL && "priv->paint_surface should set before create context.");

  cr = cairo_create(priv->paint_surface);
  return cr;
}

FRSurface *fr_draw_get_surface(FRDraw *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRDrawPrivate *priv = self->priv;

  return priv->paint_surface;
}

FRSurface* fr_draw_create_surface(FRDraw* self, SysInt width, SysInt height) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRSurface *surface;
  FRDrawPrivate *priv = self->priv;

  if (priv->window == NULL) {
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 600);
    return surface;
  }

#if SYS_OS_WIN32
  HWND hwd = fr_window_get_win32_window(priv->window);
  HDC hdc = GetDC(hwd);
  surface = cairo_win32_surface_create_with_format(hdc, CAIRO_FORMAT_ARGB32);
#elif SYS_OS_UNIX
  Window xwindow = fr_window_get_x11_window(priv->window);
  Display *ndisplay = fr_display_get_x11_display(priv->display);

  surface = cairo_xlib_surface_create(ndisplay,
      xwindow, 
      DefaultVisual(ndisplay, DefaultScreen(ndisplay)),
      width, height);
#endif

  return surface;
}

void fr_draw_frame_begin(FRDraw *self, FRRegion *region) {
  sys_return_if_fail(self != NULL);

  SysInt fbw = 0, fbh = 0;
  FRDrawPrivate *priv = self->priv;

  fr_window_get_framebuffer_size(priv->window, &fbw, &fbh);

  priv->window_surface = fr_draw_create_surface(self, fbw,fbh);
  priv->paint_surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, fbw, fbh);
}

void fr_draw_frame_end(FRDraw *self, FRRegion *region) {
  sys_return_if_fail(self != NULL);

  int n_boxes, i;
  cairo_rectangle_int_t box;

  FRDrawPrivate *priv = self->priv;
  cairo_t *cr = cairo_create(priv->window_surface);

  cairo_set_source_surface (cr, priv->paint_surface, 0, 0);

  n_boxes = cairo_region_num_rectangles(region);
  for (i = 0; i < n_boxes; i++) {
    cairo_region_get_rectangle(region, i, &box);
    cairo_rectangle(cr, box.x, box.y, box.width, box.height);
  }

  cairo_clip(cr);
  cairo_paint(cr);
  cairo_destroy(cr);

  cairo_surface_flush(priv->window_surface);

  sys_clear_pointer(&priv->window_surface, cairo_surface_destroy);
  sys_clear_pointer(&priv->paint_surface, cairo_surface_destroy);
}

/* object api */
static void fr_draw_construct(SysObject *o, FRWindow *window) {
  SYS_OBJECT_CLASS(fr_draw_parent_class)->construct(o);

  FRDraw *self = FR_DRAW(o);
  FRDrawPrivate *priv = self->priv;

  if (window) {

    priv->display = fr_window_get_display(window);
  }

  priv->window = window;
  priv->paint_surface = NULL;
  priv->window_surface = NULL;
  priv->rate_time = (1 / 60.0) * 1e3;
}

FRDraw* fr_draw_new(void) {
  return sys_object_new(FR_TYPE_DRAW, NULL);
}

FRDraw *fr_draw_new_I(FRWindow *window) {
  FRDraw *o = fr_draw_new();

  fr_draw_construct(SYS_OBJECT(o), window);

  return o;
}

static void fr_draw_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_draw_parent_class)->dispose(o);
}

static void fr_draw_class_init(FRDrawClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)fr_draw_construct;
  ocls->dispose = fr_draw_dispose;
}

void fr_draw_init(FRDraw *self) {
  self->priv = fr_draw_get_private(self);
}
