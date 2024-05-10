#include <Framework/Graph/FrCairoDraw.h>
#include <Framework/Graph/FrContext.h>
#include <Framework/Device/FrWindow.h>
#include <Framework/Device/FrDisplay.h>
#include <Framework/Device/FrWindow.h>
#include <Framework/Media/FrIMediaRender.h>
#include <Framework/Media/FrVideoFrame.h>
#include <Framework/Graph/FrSurface.h>

static void i_draw_imp(FrIDrawInterface *iface);
static void i_media_render_imp(FrIMediaRenderInterface *iface);

SYS_DEFINE_WITH_CODE(FrCairoDraw, fr_cairo_draw, FR_TYPE_DRAW_CONTEXT,
    SYS_IMPLEMENT_INTERFACE(FR_TYPE_I_DRAW, i_draw_imp));

void clear_background(cairo_t *cr) {
  cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
  cairo_paint(cr);
}

static void fr_context_rect_red_i(cairo_t *cr, SysInt x, SysInt y) {

  cairo_set_source_rgba(cr, 1.0, 0.0, 0.0, 1.0);
  cairo_rectangle(cr, x, y, 200, 100);
  cairo_stroke(cr);
}

static void cairo_overlay_i(cairo_t *cr, cairo_surface_t *surface, SysInt x, SysInt y) {

  cairo_set_source_surface(cr, surface, x, y);
  cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
}


static void i_media_render_video(FrIMediaRender *o, FrVideoFrame *frame) {
  FrCairoDraw *self = FR_CAIRO_DRAW(o);
  FrDrawContext *draw_context = FR_DRAW_CONTEXT(o);
  FrIDrawInterface *idraw_iface = FR_I_DRAW_GET_IFACE(self);

  SysUInt8 **data;
  SysInt *linesize;
  cairo_surface_t *paint_surface;
  cairo_t *window_cr;
  cairo_t *paint_cr;
  cairo_format_t format;
  SysInt width;
  SysInt height;
  int stride;

  fr_video_frame_get_frame_data(frame, &data, &linesize);
  fr_video_frame_get_out_size(frame, &width, &height);

  /* CAIRO is BGRA format */
  format = CAIRO_FORMAT_ARGB32;
  stride = linesize[0];

  paint_surface = cairo_image_surface_create_for_data(data[0],
        format,
        width,
        height,
        stride);

  window_cr = cairo_create(draw_context->idevice_surface->ctx);
  paint_cr = cairo_create(paint_surface);

#if 0
  SysChar* filename = PROJECT_DIR"/Assets/surface.png";
  cairo_status_t err;

  err = cairo_surface_write_to_png(paint_surface, filename);
  if (err != CAIRO_STATUS_SUCCESS) {
    sys_warning_N("%s: %s", filename, cairo_status_to_string(err));
    return;
  }
#endif

  cairo_save(paint_cr);
  fr_context_rect_red_i(paint_cr, 0, 10);

  cairo_restore(paint_cr);
  sys_clear_pointer(&paint_cr, cairo_destroy);

  cairo_overlay_i(window_cr, paint_surface, 0, 0);
  cairo_paint(window_cr);

  sys_clear_pointer(&window_cr, cairo_destroy);
  cairo_surface_flush(draw_context->idevice_surface->ctx);

  sys_clear_pointer(&paint_surface, cairo_surface_destroy);
}

static void i_media_render_audio(FrIMediaRender *o, FrAudioFrame *frame) {
  // FrCairoRender *self = FR_CAIRO_RENDER(o);

}

void cairo_stroke_mp_i(FrContext* self, const FrBound *bound, const FrSInt4* m4, const FrSInt4* p4) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(m4 != NULL);
  sys_return_if_fail(p4 != NULL);
  sys_return_if_fail(bound != NULL);
  cairo_t *cr = self->ctx;

  SysInt x = bound->x + m4->m3;
  SysInt y = bound->y + m4->m0;
  SysInt width = bound->width + p4->m1 + p4->m3;
  SysInt height = bound->height + p4->m0 + p4->m2;

  cairo_rectangle(cr, x, y, width, height);
  cairo_stroke(cr);
}

void cairo_context_fill_bound_i(FrContext* self, const FrBound *bound) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(bound != NULL);
  cairo_t *cr = self->ctx;

  cairo_rectangle(cr, bound->x, bound->y, bound->width, bound->height);
  cairo_fill(cr);
}

/* surface */
FrSurface* cairo_image_surface_create_i (SysInt width, SysInt height) {
  cairo_surface_t *draw_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  if(draw_surface == NULL) { return NULL; }

  FrSurface* surface = fr_surface_new();
  surface->ctx = draw_surface;

  return surface;
}

static FrSurface *surface_create_image_surface_from_surface_i(FrSurface *surface,
    SysInt width,
    SysInt height) {

  FrSurface *o = fr_surface_new();
  cairo_surface_t * ns = cairo_surface_create_similar_image(surface->ctx, CAIRO_FORMAT_ARGB32, width, height);

  o->ctx = ns;

  return o;
}

cairo_surface_t *create_native_surface(FrWindow *window, SysInt width, SysInt height) {
  cairo_surface_t* surface;

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

FrSurface* cairo_create_surface_i(FrIDevice* device, SysInt width, SysInt height) {
  sys_return_val_if_fail(device != NULL, NULL);

  FrSurface* o = fr_surface_new();

  /* only support window now */
  FrWindow *window = FR_WINDOW(device);
  o->ctx = create_native_surface(window, width, height);

  return o;
}

FrSurface* cairo_surface_create_similar_image_i(FrSurface *surface, SysInt width, SysInt height) {
  FrSurface * nsur = cairo_surface_create_similar_image(surface,
      CAIRO_FORMAT_ARGB32,
      width,
      height);

  return nsur;
}

void cairo_context_fill_background_i(FrContext *cr, SysInt width, SysInt height) {
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.5);
  cairo_rectangle(cr, 0, 0, width, height);
  cairo_paint(cr);
}

SysInt cairo_rounded_rectangle_i(FrContext* cr,
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

static void cairo_print_text_i (FrContext* cr, SysDouble x,SysDouble y, const SysChar *text) {
  cairo_move_to(cr, x, y);
  cairo_show_text(cr, text);
}

static void i_media_render_imp(FrIMediaRenderInterface *iface) {
}

static void fr_context_fill_bound(FrContext* self, const FrBound *bound) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(bound != NULL);
  cairo_t *cr = self->ctx;

  cairo_rectangle(cr, bound->x, bound->y, bound->width, bound->height);
  cairo_fill(cr);
}

void fr_context_fill_background (FrContext *self, SysInt width, SysInt height) {
  FrContext *cr = self->ctx;

  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
  cairo_rectangle(cr, 0, 0, width, height);
  cairo_paint(cr);
}

static void fr_context_rect_red(FrContext *self, SysInt x, SysInt y) {

  fr_context_rect_red_i(self->ctx, x, y);
}

void fr_context_stoke_debug (FrContext *cr, SysInt i) {
  FrContext *cr = self->ctx;
  FrIDrawInterface *idraw_iface = fr_draw_get_iface();

  cairo_set_source_rgba(cr, 1.0, 0.0, 0.0, 1.0);
  cairo_rectangle(cr, 20 + i * 5, 30 + i * 5, 200, 100);
  cairo_stroke(cr);
}

void fr_context_set_color(FrContext * self, FrColor *color) {

  fr_i_draw_set_source_rgba(self->ctx, color->r, color->g, color->b, color->a);
}

void fr_context_set_source_surface (FrContext* self, FrSurface* surface, SysDouble x, SysDouble y) {
  FrSurface *draw_surface = fr_surface_get_draw_surface(surface);

  fr_i_draw_set_source_surface(self->ctx, draw_surface, x, y);
}

void fr_context_rectangle (FrContext* self,SysDouble x,SysDouble y,SysDouble width,SysDouble height) {
  sys_return_if_fail(self != NULL);

  fr_i_draw_rectangle(self->ctx, x, y, width, height);
}

void fr_context_stroke_mp(FrContext* self, const FrBound *bound, const FrSInt4* m4, const FrSInt4* p4) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(m4 != NULL);
  sys_return_if_fail(p4 != NULL);
  sys_return_if_fail(bound != NULL);

  FrContext* cr = self->ctx;
  FrIDrawInterface* idraw_iface = fr_draw_get_iface();

  SysInt x = bound->x + m4->m3;
  SysInt y = bound->y + m4->m0;
  SysInt width = bound->width + p4->m1 + p4->m3;
  SysInt height = bound->height + p4->m0 + p4->m2;

  cairo_rectangle(cr, x, y, width, height);
  cairo_stroke(cr);
}

void fr_context_stroke(FrContext *self) {
  FrIDrawInterface* idraw_iface = fr_draw_get_iface();

  cairo_stroke(self->ctx);
}

void fr_context_clip (FrContext* self) {
  sys_return_if_fail(self != NULL);

  fr_i_draw_clip (self->ctx);
}

void fr_context_paint (FrContext* self) {
  sys_return_if_fail(self != NULL);

  fr_i_draw_paint (self->ctx);
}

void fr_context_move_to (FrContext* self,SysDouble x,SysDouble y) {
  sys_return_if_fail(self != NULL);

  fr_i_draw_move_to(self->ctx, x, y);
}

void fr_context_layout_layout(FrContext* self, PangoLayout* layout) {

  pango_cairo_update_layout(self->ctx, layout);
}

void fr_context_save(FrContext* self) {
  sys_return_if_fail(self != NULL);

  fr_i_draw_save(self->ctx);
}

void fr_context_restore(FrContext* self) {
  sys_return_if_fail(self != NULL);

  fr_i_draw_restore(self->ctx);
}

/* font */
void fr_context_draw_text(FrContext* self, PangoLayout* layout, SysInt x, SysInt y) {
  FrIDrawInterface* iface = fr_draw_get_iface();

  iface->move_to(self->ctx, x, y);
  iface->show_layout(self->ctx, layout);
}

void fr_context_print_text(FrContext* self, SysInt x, SysInt y, const SysChar *text) {
  FrIDrawInterface* iface = fr_draw_get_iface();

  iface->print_text(self->ctx, x, y, text);
}

void fr_context_show_text(FrContext* self, PangoLayout *layout,
  SysInt x, SysInt y, SysInt m1, SysInt m0) {
  FrIDrawInterface* iface = fr_draw_get_iface();

  iface->move_to(self->ctx, x + m1, y + m0);
  iface->show_layout(self->ctx, layout);
}

void fr_context_update_layout(FrContext* self, PangoLayout* layout) {

  fr_i_draw_update_layout(self->ctx, layout);
}

void fr_context_overlay(FrContext *self, FrSurface *surface, SysInt x, SysInt y) {
  FrSurface *draw_surface = fr_surface_get_draw_surface(surface);

  fr_i_draw_context_overlay(self->ctx, draw_surface, 0, 0);
}

static void i_draw_imp(FrIDrawInterface *iface) {
}


FrContext *fr_context_create(FrSurface *surface) {
  FrContext *o = fr_context_new_I(surface);

  o->ctx = cairo_create(surface->ctx);

  return o;
}

static void i_draw_imp(FrIDrawInterface *iface) {
  iface->overlay = cairo_overlay_i;
  iface->set_source_rgba = cairo_set_source_rgba;
  iface->set_source_surface = cairo_set_source_surface;
  iface->rectangle = cairo_rectangle;
  iface->paint = cairo_paint;
  iface->create = fr_context_create;
  iface->stroke = cairo_stroke;
  iface->clip = cairo_clip;
  iface->destroy = cairo_destroy;
  iface->rounded_rectangle = cairo_rounded_rectangle_i;
  iface->image_surface_create = cairo_image_surface_create_i;
  iface->create_surface = cairo_create_surface_i;
  iface->surface_create_similar_image = cairo_surface_create_similar_image_i;
  iface->surface_flush = cairo_surface_flush;
  iface->surface_destroy = cairo_surface_destroy;
  iface->print_text = cairo_print_text_i;
  // iface->fill_bound = fr_context_fill_bound;
}

/* object api */
FrDraw* fr_cairo_draw_new(void) {
  return sys_object_new(FR_TYPE_CAIRO_DRAW, NULL);
}

FrDraw *fr_cairo_draw_new_I(void) {
  FrDraw *o = fr_cairo_draw_new();

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
