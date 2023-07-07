#ifndef __TEST_CAIRO__
#define __TEST_CAIRO__

#include <CstDemo/Demo.h>

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>
#ifdef SYS_OS_UNIX
  #define GLFW_EXPOSE_NATIVE_X11 1
  #define CAIRO_HAS_GLX_FUNCTIONS 1
  #include <X11/Xlib.h>
  #include <X11/Xutil.h>

  #include <cairo/cairo-xlib.h>
  #include <cairo/cairo-xcb.h>
#elif defined(SYS_OS_WIN32)
  #define GLFW_EXPOSE_NATIVE_WIN32
  #define GLFW_EXPOSE_NATIVE_WGL 1
  #define CAIRO_HAS_WGL_FUNCTIONS 1
  #include <cairo/cairo-win32.h>
#endif

#include <glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

typedef  struct _cairo_pattern FRPattern;
typedef  struct _cairo_region  FRRegion;
typedef  struct _cairo_rectangle_int FRRect;
typedef  struct _cairo_surface FRSurface;
typedef  struct _cairo FRContext;


SYS_BEGIN_DECLS

void test_cairo_init(int argc, char *argv[]);

SYS_END_DECLS

#endif

