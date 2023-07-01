#ifndef __TEST_GLFEW__
#define __TEST_GLFEW__

#include "SysOs.h"
#include "glib.h"
#include "unity.h"
#include <gio/gio.h>
#include <cairo/cairo.h>

#ifdef SYS_OS_UNIX
#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>
#else
#include <cairo/cairo-win32.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#ifdef SYS_OS_WIN32
  #define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <GLFW/glfw3native.h>

G_BEGIN_DECLS

void test_glfw_init(void);

G_END_DECLS

#endif
