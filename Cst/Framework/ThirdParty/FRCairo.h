#ifndef __FR_CAIRO_H__
#define __FR_CAIRO_H__

#include <System/SysCore.h>

SYS_BEGIN_DECLS

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>

#ifdef SYS_OS_UNIX
  #include <X11/Xlib.h>
  #define CAIRO_HAS_GLX_FUNCTIONS 1

  #include <cairo/cairo-xlib.h>
  #include <cairo/cairo-xcb.h>
#elif defined(SYS_OS_WIN32)
  #define CAIRO_HAS_WGL_FUNCTIONS 1
  #include <cairo/cairo-win32.h>
#endif

typedef  struct  _FRDrawContext   FRDraw;
typedef  struct _cairo_pattern FRPattern;
typedef  struct _cairo_region  FRRegion;
typedef  struct _cairo_rectangle_int FRRect;
typedef  struct _cairo_surface FRSurface;
typedef  struct _cairo FRContext;

SYS_END_DECLS

#endif

