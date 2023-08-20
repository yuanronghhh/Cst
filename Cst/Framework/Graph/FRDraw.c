#include <Framework/Graph/FRDraw.h>
#include <Framework/Device/FRDisplay.h>
#include <Framework/Device/FRWindow.h>


struct _FRDrawPrivate {
  SysPointer v;

  FRContext *cr;
  FRSurface *window_surface;
  FRSurface * paint_surface;
  FRDisplay *display;
  FRWindow *window;
  SysBool is_painting;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRDraw, fr_draw, SYS_TYPE_OBJECT);

SysBool fr_draw_frame_need_draw(FRDraw *self) {
  sys_return_val_if_fail(self != NULL, false);

  FRDrawPrivate *priv = self->priv;

  return !priv->is_painting;
}

/* FRDraw */
void fr_draw_surface_flush(FRDraw *self) {
  sys_return_if_fail(self != NULL);

  FRDrawPrivate *priv = self->priv;

  FRContext *cr = cairo_create(priv->window_surface);
  cairo_set_source_surface(cr, priv->paint_surface, 0, 0);
  cairo_surface_flush(priv->window_surface);
}

void fr_draw_stroke_mp(FRDraw* self, const FRRect *bound, FRSInt4* m4, FRSInt4* p4) {

  sys_return_if_fail(self != NULL);
  sys_return_if_fail(m4 != NULL);
  sys_return_if_fail(p4 != NULL);
  sys_return_if_fail(bound != NULL);

  FRDrawPrivate *priv = self->priv;
  FRContext *cr = priv->cr;

  SysInt x = bound->x + m4->m0;
  SysInt y = bound->y + m4->m0;
  SysInt width = bound->width + p4->m1 + p4->m3;
  SysInt height = bound->height + p4->m0 + p4->m2;

  fr_context_rectangle(cr, x, y, width, height);
  fr_context_stroke(cr);
}

void fr_draw_fill_rectangle(FRDraw* self, const FRRect *bound) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(bound != NULL);

  FRDrawPrivate *priv = self->priv;
  FRContext *cr = priv->cr;

  fr_context_rectangle(cr, bound->x, bound->y, bound->width, bound->height);
  fr_context_fill(cr);
}

void fr_draw_get_size(FRDraw *self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);

  FRDrawPrivate *priv = self->priv;

  fr_window_get_framebuffer_size(priv->window, width, height);
}

FRContext* fr_draw_get_cr(FRDraw* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRDrawPrivate* priv = self->priv;

  return priv->cr;
}

FRContext* fr_draw_create_cr(FRDraw* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRContext *cr;
  FRDrawPrivate *priv = self->priv;

  sys_assert(priv->paint_surface != NULL && "paint_surface should set before create context.");
  sys_assert(priv->window_surface != NULL && "window_surface should set before create context.");

  cr = cairo_create(priv->window_surface);

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
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    return surface;
  }

#if SYS_OS_WIN32
  HWND hwd = fr_window_get_win32_window(priv->window);
  HDC hdc = GetDC(hwd);
  surface = cairo_win32_surface_create_with_format(hdc, CAIRO_FORMAT_ARGB32);
#elif SYS_OS_UNIX
  FRDisplay *display = fr_window_get_display(priv->window);
  Window xwindow = fr_window_get_x11_window(priv->window);
  Display *ndisplay = fr_display_get_x11_display(display);
  int nscreen = DefaultScreen(ndisplay);
  Visual *nvisual = DefaultVisual(ndisplay, nscreen);

  surface = cairo_xlib_surface_create(ndisplay,
      xwindow,
      nvisual,
      width, height);
#endif

  return surface;
}

void fr_draw_frame_begin(FRDraw *self, FRRegion *region) {
  sys_return_if_fail(self != NULL);

  SysInt fbw = 0, fbh = 0;
  FRDrawPrivate *priv = self->priv;

  fr_window_get_framebuffer_size(priv->window, &fbw, &fbh);

  // sys_debug_N("%d,%d", fbw, fbh);

  priv->window_surface = fr_draw_create_surface(self, fbw, fbh);
  priv->paint_surface = cairo_surface_create_similar_image(priv->window_surface, CAIRO_FORMAT_ARGB32, fbw, fbh);

#if SYS_OS_WIN32
  FRContext* cr = cairo_create(priv->window_surface);

  cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
  cairo_paint(cr);
  cairo_destroy(cr);
#endif

  sys_assert(priv->cr == NULL && "draw cr should be NULL when fr_draw_frame_begin, missing fr_draw_frame_end ?");

  priv->cr = cairo_create(priv->paint_surface);
  priv->is_painting = true;
}

void fr_draw_frame_end(FRDraw *self, FRRegion *region) {
  sys_return_if_fail(self != NULL);

  int n_boxes, i;
  cairo_rectangle_int_t box;
  FRContext* cr;

  FRDrawPrivate *priv = self->priv;

  cr = cairo_create(priv->window_surface);
  cairo_set_source_surface(cr, priv->paint_surface, 0, 0);

  n_boxes = cairo_region_num_rectangles(region);
  for (i = 0; i < n_boxes; i++) {
    cairo_region_get_rectangle(region, i, &box);
    cairo_rectangle(cr, box.x, box.y, box.width, box.height);
  }

  cairo_clip(cr);
  cairo_paint(cr);
  cairo_destroy(cr);

  sys_clear_pointer(&priv->cr, cairo_destroy);
  cairo_surface_flush(priv->window_surface);

  sys_clear_pointer(&priv->window_surface, cairo_surface_destroy);
  sys_clear_pointer(&priv->paint_surface, cairo_surface_destroy);

  priv->is_painting = false;
}

/* object api */
static void fr_draw_construct(FRDraw *self, FRWindow *window) {
  FRDrawPrivate *priv = self->priv;

  priv->window = window;
  priv->window_surface = NULL;
}

FRDraw* fr_draw_new(void) {
  return sys_object_new(FR_TYPE_DRAW, NULL);
}

FRDraw *fr_draw_new_I(FRWindow *window) {
  FRDraw *o = fr_draw_new();

  fr_draw_construct(o, window);

  return o;
}

static void fr_draw_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_draw_parent_class)->dispose(o);
}

static void fr_draw_class_init(FRDrawClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_draw_dispose;
}

void fr_draw_init(FRDraw *self) {
  self->priv = fr_draw_get_private(self);
}
