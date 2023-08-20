#ifndef __TEST_GLFW__
#define __TEST_GLFW__

#include <CstDemo/Demo.h>

#ifdef SYS_OS_UNIX
  #include <X11/Xlib.h>
  #include <X11/Xutil.h>
  #define CAIRO_HAS_GLX_FUNCTIONS 1
  #define GLFW_EXPOSE_NATIVE_X11 1

  #include <cairo/cairo-xlib.h>
  #include <cairo/cairo-xcb.h>
#elif defined(SYS_OS_WIN32)
  #define CAIRO_HAS_WGL_FUNCTIONS 1
  #define GLFW_EXPOSE_NATIVE_WIN32
  #define GLFW_EXPOSE_NATIVE_WGL 1
  #include <cairo/cairo-win32.h>
#endif

#include <glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>

SYS_BEGIN_DECLS

void test_glfw_init(int argc, char *argv[]);

SYS_END_DECLS

#endif

