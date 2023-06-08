#include <Framework/Graph/FRDraw.h>
#include <Framework/Device/FRDisplay.h>
#include <Framework/Device/FRWindow.h>


struct _FRDrawPrivate {
  SysPointer v;

  SysInt64 last_clock;
  SysDouble rate_time;

  /* not support double buffer now */
  FRSurface *surface;
  FRDisplay *display;
  FRWindow *window;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRDraw, fr_draw, SYS_TYPE_OBJECT);

SysBool fr_draw_frame_need_draw(FRDraw *self) {
  sys_return_val_if_fail(self != NULL, false);

  FRDrawPrivate *priv = self->priv;
  SysInt64 current = sys_get_monoic_time();

  if (priv->last_clock == 0) {
    priv->last_clock = current;
    return true;
  }

  SysBool r = (current - priv->last_clock) > priv->rate_time;
  
  printf("%lld\t%lld\t%lld\t%d\n", current, priv->last_clock, (current - priv->last_clock), r);

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

  sys_assert(priv->surface != NULL && "priv->surface should set before create context.");

  cr = cairo_create(priv->surface);
  return cr;
}

FRSurface *fr_draw_get_surface(FRDraw *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRDrawPrivate *priv = self->priv;

  return priv->surface;
}

FRSurface* fr_draw_create_surface(FRDraw* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  SysInt fbw = 0, fbh = 0;
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

  fr_window_get_framebuffer_size(priv->window, &fbw, &fbh);

  surface = cairo_xlib_surface_create(ndisplay, xwindow, DefaultVisual(ndisplay, DefaultScreen(ndisplay)), fbw, fbh);
#endif

  return surface;
}

void fr_draw_frame_begin(FRDraw *self, FRRegion *region) {
  sys_return_if_fail(self != NULL);

  cairo_t *cr;
  int n_boxes, i;
  cairo_rectangle_int_t box;
  cairo_surface_t *surface;

  FRDrawPrivate *priv = self->priv;

  surface = priv->surface = fr_draw_create_surface(self);

  cr = cairo_create(surface);

  cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 1.0);
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

  n_boxes = cairo_region_num_rectangles(region);

  for (i = 0; i < n_boxes; i++) {
    cairo_region_get_rectangle(region, i, &box);
    cairo_rectangle(cr, box.x, box.y, box.width, box.height);
  }

  cairo_clip(cr);
  cairo_paint(cr);
  cairo_destroy(cr);
}

void fr_draw_frame_end(FRDraw *self, FRRegion *region) {
  sys_return_if_fail(self != NULL);

  FRDrawPrivate *priv = self->priv;

  cairo_surface_flush(priv->surface);
  sys_clear_pointer(&priv->surface, cairo_surface_destroy);
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
  priv->surface = NULL;
  priv->rate_time = (1 / 24.0) * 1e3;
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
  FRDraw *self = FR_DRAW(o);
  FRDrawPrivate *priv = self->priv;

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
