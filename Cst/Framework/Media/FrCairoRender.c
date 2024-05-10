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

static SysInt offset = 0;

static void i_media_render_imp(FrIMediaRenderInterface *iface) {
  iface->render_video = i_media_render_video;
  iface->render_audio = i_media_render_audio;
}

/* object api */
static void fr_cairo_render_construct_i(FrCairoRender *self, FrDrawContext *draw_context) {
  self->draw_context = sys_object_ref(draw_context);
}

FrCairoRender* fr_cairo_render_new(void) {
  return sys_object_new(FR_TYPE_CAIRO_RENDER, NULL);
}

FrCairoRender *fr_cairo_render_new_I(FrDrawContext *draw_context) {
  FrCairoRender *o = fr_cairo_render_new();

  fr_cairo_render_construct_i(o, draw_context);

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
