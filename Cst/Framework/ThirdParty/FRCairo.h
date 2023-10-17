#ifndef __FR_CAIRO_H__
#define __FR_CAIRO_H__

#include <System/SysCore.h>

SYS_BEGIN_DECLS

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>

#ifdef SYS_OS_UNIX
  #include <X11/Xlib.h>
  #include <X11/Xutil.h>
  #define CAIRO_HAS_GLX_FUNCTIONS 1

  #include <cairo/cairo-xlib.h>
  #include <cairo/cairo-xcb.h>
#elif defined(SYS_OS_WIN32)
  #define CAIRO_HAS_WGL_FUNCTIONS 1
  #include <cairo/cairo-win32.h>
#endif

void fr_cairo_demo(cairo_t* cr);

SYS_END_DECLS

#endif

