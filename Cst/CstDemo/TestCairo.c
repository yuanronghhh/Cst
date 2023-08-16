#include <CstDemo/TestCairo.h>

FRSurface* create_surface(GLFWwindow *gwindow, SysInt width, SysInt height) {
  FRSurface* surface;

#if SYS_OS_WIN32
  HWND hwd = glfwGetWin32Window(gwindow);
  HDC hdc = GetDC(hwd);
  surface = cairo_win32_surface_create_with_format(hdc, CAIRO_FORMAT_RGB24);
#elif SYS_OS_UNIX
  Window xwindow = fr_window_get_x11_window(priv->window);
  Display* ndisplay = fr_display_get_x11_display(priv->display);

  surface = cairo_xlib_surface_create(ndisplay,
    xwindow,
    DefaultVisual(ndisplay, DefaultScreen(ndisplay)),
    width, height);
#endif

  return surface;
}

void cairo_test_flex_layout(void) {
}

static void cairo_basic(void) {
  sys_setup();

  SysInt width = 800;
  SysInt height = 600;
  SysChar *title = "hello";

  if (!glfwInit())
    exit(EXIT_FAILURE);

  GLFWwindow *gwindow = glfwCreateWindow(width, height, title, NULL, NULL);
  cairo_surface_t* sur = create_surface(gwindow, 800, 600);

  cairo_t* cr = cairo_create(sur);

  while (!glfwWindowShouldClose(gwindow)) {
    glfwWaitEvents();

    cairo_rectangle(cr, 0, 0, 200, 100);
    cairo_stroke(cr);
  }

  sys_teardown();
}

void test_cairo_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(cairo_basic);
  }
  UNITY_END();
}
