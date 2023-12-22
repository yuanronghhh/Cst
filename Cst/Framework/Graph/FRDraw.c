#include <Framework/Graph/FRDraw.h>
#include <Framework/Device/FRDisplay.h>
#include <Framework/Device/FRWindow.h>


SYS_DEFINE_TYPE(FRDraw, fr_draw, SYS_TYPE_OBJECT);

SysBool fr_draw_frame_need_draw(FRDraw *self) {
  sys_return_val_if_fail(self != NULL, false);

  return !self->is_painting;
}

/* FRDraw */
void fr_draw_surface_flush(FRDraw *self) {
  sys_return_if_fail(self != NULL);

  FRContext *cr = cairo_create(self->window_surface);
  cairo_set_source_surface(cr, self->paint_surface, 0, 0);
  cairo_surface_flush(self->window_surface);
}

void fr_draw_stroke_mp(FRDraw* self, const FRRect *bound, const FRSInt4* m4, const FRSInt4* p4) {

  sys_return_if_fail(self != NULL);
  sys_return_if_fail(m4 != NULL);
  sys_return_if_fail(p4 != NULL);
  sys_return_if_fail(bound != NULL);

  FRContext *cr = self->cr;

  SysInt x = bound->x + m4->m3;
  SysInt y = bound->y + m4->m0;
  SysInt width = bound->width + p4->m1 + p4->m3;
  SysInt height = bound->height + p4->m0 + p4->m2;

  cairo_rectangle(cr, x, y, width, height);
  cairo_stroke(cr);
}

void fr_draw_fill_rectangle(FRDraw* self, const FRRect *bound) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(bound != NULL);

  FRContext *cr = self->cr;

  cairo_rectangle(cr, bound->x, bound->y, bound->width, bound->height);
  cairo_fill(cr);
}

FRContext* fr_draw_create_cr(FRDraw* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRContext *cr;
  sys_assert(self->paint_surface != NULL && "paint_surface should set before create context.");
  sys_assert(self->window_surface != NULL && "window_surface should set before create context.");

  cr = cairo_create(self->window_surface);

  return cr;
}

FRSurface *fr_draw_get_surface(FRDraw *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->paint_surface;
}

static FRSurface* create_surface(FRWindow *window, SysInt width, SysInt height) {
  FRSurface *surface;

  if (window == NULL) {
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);

  } else {

#if SYS_OS_WIN32
    HWND hwd = fr_window_get_win32_window(window);
    HDC hdc = GetDC(hwd);
    surface = cairo_win32_surface_create_with_format(hdc, CAIRO_FORMAT_ARGB32);
#elif SYS_OS_UNIX
    FRDisplay *display = fr_window_get_display(window);
    Window xwindow = fr_window_get_x11_window(window);
    Display *ndisplay = fr_display_get_x11_display(display);
    int nscreen = DefaultScreen(ndisplay);
    Visual *nvisual = DefaultVisual(ndisplay, nscreen);

    surface = cairo_xlib_surface_create(ndisplay,
        xwindow,
        nvisual,
        width, height);
#endif

  }

  return surface;
}

static  FRSurface* create_image_surface_from_surface(FRSurface *surface, SysInt width, SysInt height) {
  FRSurface * nsur =cairo_surface_create_similar_image(surface, CAIRO_FORMAT_ARGB32, 
    width, height);

  return nsur;
}

void fr_context_fill_background(FRContext *cr, SysInt width, SysInt height) {
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
  cairo_rectangle(cr, 0, 0, width, height);
  cairo_fill(cr);
}


void fr_draw_set_color(FRDraw *self, FRColor *color) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->cr != NULL);

  cairo_set_source_rgba(self->cr, color->r, color->g, color->b, color->a);
}

void fr_draw_layout_layout(FRDraw *self, FRDrawLayout *layout) {
  sys_return_if_fail(self != NULL);

  pango_cairo_update_layout(self->cr, layout);
}

void fr_draw_frame_begin(FRDraw *self, FRRegion *region) {
  sys_return_if_fail(self != NULL);

  SysInt fbw = 0, fbh = 0;
  fr_window_get_framebuffer_size(self->window, &fbw, &fbh);

  self->window_surface = create_surface(self->window, fbw, fbh);
  self->paint_surface = create_image_surface_from_surface(self->window_surface, fbw, fbh);

  sys_assert(self->cr == NULL && "draw cr should be NULL when fr_draw_frame_begin, missing fr_draw_frame_end ?");

  self->cr = cairo_create(self->paint_surface);
  fr_context_fill_background(self->cr, fbw, fbh);

  self->is_painting = true;
}

void fr_draw_frame_end(FRDraw *self, FRRegion *region) {
  sys_return_if_fail(self != NULL);

  int n_boxes, i;
  cairo_rectangle_int_t box;
  FRContext* cr;

  cr = cairo_create(self->window_surface);
  cairo_set_source_surface(cr, self->paint_surface, 0, 0);

  n_boxes = cairo_region_num_rectangles(region);
  for (i = 0; i < n_boxes; i++) {
    cairo_region_get_rectangle(region, i, &box);
    cairo_rectangle(cr, box.x, box.y, box.width, box.height);
  }

  cairo_clip(cr);
  cairo_paint(cr);
  cairo_destroy(cr);

  sys_clear_pointer(&self->cr, cairo_destroy);
  cairo_surface_flush(self->window_surface);

  sys_clear_pointer(&self->window_surface, cairo_surface_destroy);
  sys_clear_pointer(&self->paint_surface, cairo_surface_destroy);

  self->is_painting = false;
}

void fr_draw_save(FRDraw* self) {
  cairo_save(self->cr);
}

void fr_draw_restore(FRDraw* self) {
  cairo_restore(self->cr);
}

/* text render */
void fr_draw_show_text(FRDraw* self, FRDrawLayout *layout, SysInt x, SysInt y, SysInt m1, SysInt m0) {
  FRContext* cr = self->cr;

  cairo_move_to(cr, x + m1, y + m0);
  pango_cairo_show_layout(cr, layout);
}

/* object api */
static void fr_draw_construct(FRDraw *self, FRWindow *window) {
  self->window = window;
  self->window_surface = NULL;
  sys_object_ref(window);
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
  FRDraw* self = FR_DRAW(o);

  if (self->window) {

    sys_clear_pointer(&self->window, _sys_object_unref);
  }

  SYS_OBJECT_CLASS(fr_draw_parent_class)->dispose(o);
}

static void fr_draw_class_init(FRDrawClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_draw_dispose;
}

void fr_draw_init(FRDraw *self) {
}
