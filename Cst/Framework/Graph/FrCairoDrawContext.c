#include <Framework/Graph/FrCairoDrawContext.h>
#include <Framework/Graph/FrIDraw.h>
#include <Framework/Graph/FrContext.h>
#include <Framework/Graph/FrSurface.h>
#include <Framework/Device/FrWindow.h>
#include <Framework/Device/FrDisplay.h>
#include <Framework/Device/FrWindow.h>
#include <Framework/Device/FrIDevice.h>
#include <Framework/Media/FrIMediaRender.h>
#include <Framework/Media/FrVideoFrame.h>

static void i_draw_imp(FrIDrawInterface *iface);
static void i_media_render_imp(FrIMediaRenderInterface *iface);

SYS_DEFINE_WITH_CODE(FrCairoDrawContext, fr_cairo_draw_context, FR_TYPE_DRAW_CONTEXT,
    SYS_IMPLEMENT_INTERFACE(FR_TYPE_I_DRAW, i_draw_imp)
    SYS_IMPLEMENT_INTERFACE(FR_TYPE_I_MEDIA_RENDER, i_media_render_imp));

static void cairo_clear_background(cairo_t *cr) {
  cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
  cairo_paint(cr);
}

static void cairo_overlay_i(cairo_t *cr, cairo_surface_t *surface, SysInt x, SysInt y) {

  cairo_set_source_surface(cr, surface, x, y);
  cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
}

static void fr_surface_flush(FrSurface *self) {
  sys_return_if_fail(self != NULL);

  cairo_surface_flush(self->ctx);
}

static void fr_surface_destroy(FrSurface *self) {
  sys_return_if_fail(self != NULL);

  cairo_surface_destroy(self->ctx);
}

static FrSurface *fr_surface_resize_surface(FrSurface *self,
    FrSurface *device_surface,
    SysInt width,
    SysInt height) {

  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(device_surface != NULL, NULL);
  cairo_format_t format = CAIRO_FORMAT_ARGB32;

  cairo_surface_t * new_surface =
    cairo_surface_create_similar_image(self->ctx, format, width, height);
  if(new_surface == NULL) { return NULL; }

  sys_clear_pointer(&self->ctx, cairo_surface_destroy);
  self->ctx = new_surface;

  return self;
}

static void fr_context_set_color(FrContext* self, FrColor* color) {

  cairo_set_source_rgba(self->ctx, color->r, color->g, color->b, color->a);
}

static void cairo_rectangle_red_i(cairo_t* cr, SysInt x, SysInt y) {
  FrColor color = { 1.0, 0.0, 0.0, 1.0 };

  cairo_set_source_rgba(cr, color.r, color.g, color.b, color.a);
  cairo_rectangle(cr, x, y, 200, 100);
  cairo_stroke(cr);
}

static void i_media_render_video(FrIMediaRender *o, FrVideoFrame *frame) {
  FrDrawContext *draw_context = FR_DRAW_CONTEXT(o);

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

  window_cr = cairo_create(draw_context->device_surface->ctx);
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
  cairo_rectangle_red_i(paint_cr, 0, 10);
  sys_clear_pointer(&paint_cr, cairo_destroy);

  cairo_overlay_i(window_cr, paint_surface, 0, 0);
  cairo_paint(window_cr);

  sys_clear_pointer(&window_cr, cairo_destroy);
  cairo_surface_flush(draw_context->device_surface->ctx);

  sys_clear_pointer(&paint_surface, cairo_surface_destroy);
}

static void i_media_render_audio(FrIMediaRender *o, FrAudioFrame *frame) {

}

static void cairo_stroke_mp_i(FrContext* self, const FrBound *bound, const FrSInt4* m4, const FrSInt4* p4) {
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

static void cairo_context_fill_bound_i(FrContext* self, const FrBound *bound) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(bound != NULL);
  cairo_t *cr = self->ctx;

  cairo_rectangle(cr, bound->x, bound->y, bound->width, bound->height);
  cairo_fill(cr);
}

/* surface */
static void fr_surface_create(FrSurface* o, FrSurfaceContext *info) {
  cairo_surface_t *draw_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
      info->width, 
      info->height);
  if(draw_surface == NULL) { return; }

  o->ctx = draw_surface;
}

static void surface_create_image_surface_from_surface_i(
    FrSurface *o, 
    FrSurface *surface,
    SysInt width,
    SysInt height) {

  cairo_surface_t * ns = cairo_surface_create_similar_image(surface->ctx, CAIRO_FORMAT_ARGB32, width, height);
  o->ctx = ns;
}

static cairo_surface_t *create_cairo_surface(FrWindow *window, SysInt width, SysInt height) {
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

static FrSurface* fr_surface_create_similar_image(FrSurface *surface, SysInt width, SysInt height) {
  FrSurface* o = fr_surface_new();

  o->ctx = cairo_surface_create_similar_image(surface->ctx,
      CAIRO_FORMAT_ARGB32,
      width,
      height);

  return o;
}

static void cairo_fill_background_i(cairo_t *cr, SysInt width, SysInt height) {

  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.5);
  cairo_rectangle(cr, 0, 0, width, height);
  cairo_paint(cr);
}

static void fr_context_fill_background(FrContext *self, SysInt width, SysInt height) {
  cairo_t *cr = self->ctx;

  cairo_fill_background_i(cr, width, height);
}

static SysInt cairo_rounded_rectangle_i(cairo_t* cr,
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

static SysInt fr_context_rounded_rectangle(FrContext* self,
  SysDouble x, SysDouble y, SysDouble w, SysDouble h, 
  SysDouble radius) {

  cairo_t *cr = self->ctx;

  return cairo_rounded_rectangle_i(cr, x, y, w, h, radius);
}

static void cairo_show_text_i (cairo_t *cr,
    SysDouble x,
    SysDouble y, 
    const SysChar *text) {

  cairo_move_to(cr, x, y);
  cairo_show_text(cr, text);
}

static void i_media_render_imp(FrIMediaRenderInterface *iface) {
  iface->render_video = i_media_render_video; 
  iface->render_audio = i_media_render_audio; 
}

static void fr_context_set_source_surface (FrContext* self, FrSurface* surface, SysDouble x, SysDouble y) {

  cairo_set_source_surface(self->ctx, surface->ctx, x, y);
}

static void fr_context_rectangle (FrContext* self, SysDouble x,SysDouble y,SysDouble width,SysDouble height) {
  sys_return_if_fail(self != NULL);

  cairo_rectangle(self->ctx, x, y, width, height);
}

static void fr_context_stroke(FrContext *self) {

  cairo_stroke(self->ctx);
}

static void fr_context_clip (FrContext* self) {
  sys_return_if_fail(self != NULL);

  cairo_clip (self->ctx);
}

static void fr_context_paint (FrContext* self) {
  sys_return_if_fail(self != NULL);

  cairo_paint (self->ctx);
}

static void fr_context_move_to (FrContext* self,SysDouble x,SysDouble y) {
  sys_return_if_fail(self != NULL);

  cairo_move_to(self->ctx, x, y);
}

static void fr_context_layout_layout(FrContext* self, PangoLayout* layout) {

  pango_cairo_update_layout(self->ctx, layout);
}

static void fr_context_save(FrContext* self) {
  sys_return_if_fail(self != NULL);

  cairo_save(self->ctx);
}

static void fr_context_restore(FrContext* self) {
  sys_return_if_fail(self != NULL);

  cairo_restore(self->ctx);
}

/* font */
static void fr_context_show_text(FrContext* self, const SysChar *text) {

  cairo_show_text(self->ctx, text);
}

static void fr_context_show_layout(FrContext* self, PangoLayout *layout) {

  pango_cairo_show_layout(self->ctx, layout);
}

static void fr_context_overlay(FrContext *self, FrSurface *surface, SysInt x, SysInt y) {

  cairo_overlay_i(self->ctx, surface->ctx, 0, 0);
}

static FrContext *fr_context_create(FrSurface *surface) {
  FrContext *o = fr_context_new_I(surface);

  o->ctx = cairo_create(surface->ctx);

  return o;
}

static void fr_context_destroy(FrContext *self) {

  cairo_destroy(self->ctx);
}

static void i_draw_imp(FrIDrawInterface *iface) {
  iface->overlay = fr_context_overlay;
  iface->set_color = fr_context_set_color;
  iface->set_source_surface = fr_context_set_source_surface;
  iface->rectangle = fr_context_rectangle;
  iface->paint = fr_context_paint;
  iface->create = fr_context_create;
  iface->stroke = fr_context_stroke;
  iface->clip = fr_context_clip;
  iface->destroy = fr_context_destroy;
  iface->rounded_rectangle = fr_context_rounded_rectangle;
  iface->surface_create_similar_image = fr_surface_create_similar_image;
  iface->surface_flush = fr_surface_flush;
  iface->surface_destroy = fr_surface_destroy;
  iface->show_layout = fr_context_show_layout;
  iface->paint = fr_context_paint;
  iface->surface_create = fr_surface_create;
  iface->save = fr_context_save;
  iface->restore = fr_context_restore;
  iface->show_text = fr_context_show_text;
}

static void fr_cairo_context_construct_i(FrDrawContext* o, FrDevice* device) {

  FR_DRAW_CONTEXT_CLASS(fr_cairo_draw_context_parent_class)->construct(o, device);
}

/* object api */
FrDrawContext* fr_cairo_draw_context_new(void) {
  return sys_object_new(FR_TYPE_CAIRO_DRAW_CONTEXT, NULL);
}

FrDrawContext *fr_cairo_draw_context_new_I(FrDevice *device) {
  FrDrawContext *o = fr_cairo_draw_context_new();

  fr_cairo_context_construct_i(o, device);

  return o;
}

static void fr_cairo_draw_context_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_cairo_draw_context_parent_class)->dispose(o);
}

static void fr_cairo_draw_context_class_init(FrCairoDrawContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrDrawContextClass* dcls = FR_DRAW_CONTEXT_CLASS(cls);

  dcls->construct = fr_cairo_context_construct_i;
  ocls->dispose = fr_cairo_draw_context_dispose;
}

void fr_cairo_draw_context_init(FrCairoDrawContext* self) {
}
