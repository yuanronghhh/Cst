#include <Framework/Media/FrCairoRender.h>
#include <Framework/Media/FrVideoFrame.h>
#include <Framework/Media/FrMedia.h>
#include <Framework/Media/FrIMediaRender.h>

static void i_media_render_imp(FrIMediaRenderInterface *iface);

SYS_DEFINE_WITH_CODE(FrCairoRender, fr_cairo_render, SYS_TYPE_OBJECT,
    SYS_IMPLEMENT_INTERFACE(FR_TYPE_I_MEDIA_RENDER, i_media_render_imp));

cairo_surface_t* create_surface(GLFWwindow *gwindow, SysInt width, SysInt height) {
  cairo_surface_t* surface;

#if SYS_OS_WIN32
  HWND hwd = glfwGetWin32Window(gwindow);
  HDC hdc = GetDC(hwd);
  surface = cairo_win32_surface_create_with_format(hdc, CAIRO_FORMAT_ARGB32);
#elif SYS_OS_UNIX
  Window xwindow = glfwGetX11Window(gwindow);
  Display* xdisplay = glfwGetX11Display();
  int screen_num = DefaultScreen(xdisplay);
  Visual *visual = DefaultVisual(xdisplay, screen_num);

  surface = cairo_xlib_surface_create(xdisplay,
    xwindow,
    visual,
    width, height);
#endif

  return surface;
}

void draw_rect_red(cairo_t *cr, SysInt x, SysInt y) {
  cairo_set_source_rgba(cr, 1.0, 0.0, 0.0, 1.0);
  cairo_rectangle(cr, x, y, 200, 100);
  cairo_stroke(cr);
}

void clear_background(cairo_t *cr) {
  cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
  cairo_paint(cr);
}

static SysInt offset = 0;

static void i_media_render_video(FrIMediaRender *o, FrVideoFrame *frame) {
  FrCairoRender *self = FR_CAIRO_RENDER(o);
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
  window_cr = cairo_create(self->window_surface);
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
  draw_rect_red(paint_cr, 0, offset);
  cairo_restore(paint_cr);
  sys_clear_pointer(&paint_cr, cairo_destroy);

  cairo_set_source_surface(window_cr, paint_surface, 0, 0);
  cairo_set_operator(window_cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint(window_cr);

  sys_clear_pointer(&window_cr, cairo_destroy);
  cairo_surface_flush(self->window_surface);

  sys_clear_pointer(&paint_surface, cairo_surface_destroy);
}

static void i_media_render_audio(FrIMediaRender *o, FrAudioFrame *frame) {
  FrCairoRender *self = FR_CAIRO_RENDER(o);

}

static void i_media_render_imp(FrIMediaRenderInterface *iface) {
  iface->render_video = i_media_render_video;
  iface->render_audio = i_media_render_audio;
}

/* object api */
static void fr_cairo_render_construct_i(FrCairoRender *self, GLFWwindow *gwindow) {
  self->gwindow = gwindow;
  self->window_surface = create_surface(gwindow, 800, 600);

  cairo_t* cr = cairo_create(self->window_surface);
  clear_background(cr);
  cairo_destroy(cr);
}

FrCairoRender* fr_cairo_render_new(void) {
  return sys_object_new(FR_TYPE_CAIRO_RENDER, NULL);
}

FrCairoRender *fr_cairo_render_new_I(GLFWwindow *gwindow) {
  FrCairoRender *o = fr_cairo_render_new();

  fr_cairo_render_construct_i(o, gwindow);

  return o;
}

static void fr_cairo_render_dispose(SysObject* o) {
  FrCairoRender *self = FR_CAIRO_RENDER(o);

  sys_clear_pointer(&self->window_surface, cairo_surface_destroy);

  SYS_OBJECT_CLASS(fr_cairo_render_parent_class)->dispose(o);
}

static void fr_cairo_render_class_init(FrCairoRenderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_cairo_render_dispose;
}

void fr_cairo_render_init(FrCairoRender* self) {
}
