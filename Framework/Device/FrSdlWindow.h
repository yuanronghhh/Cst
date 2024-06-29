#ifndef __FR_SDL_WINDOW_H__
#define __FR_SDL_WINDOW_H__

#include <Framework/Device/FrWindow.h>

SYS_BEGIN_DECLS

#define FR_TYPE_SDL_WINDOW (fr_sdl_window_get_type())
#define FR_SDL_WINDOW(o) ((FrSdlWindow* )sys_object_cast_check(o, FR_TYPE_SDL_WINDOW))
#define FR_SDL_WINDOW_CLASS(o) ((FrSdlWindowClass *)sys_class_cast_check(o, FR_TYPE_SDL_WINDOW))
#define FR_SDL_WINDOW_GET_CLASS(o) sys_instance_get_class(o, FrSdlWindowClass)

struct _FrSdlWindowClass {
  FrWindowClass parent;
};

struct _FrSdlWindow {
  FrWindow parent;
  /* <private> */
  SDL_Window *gwindow;
};

SYS_API SysType fr_sdl_window_get_type(void);
SYS_API FrWindow* fr_sdl_window_new(void);

void fr_sdl_window_setup(void);
void fr_sdl_window_iface_setup(FrIWindowInterface *iface);
void fr_sdl_window_teardown(void);

SYS_END_DECLS

#endif

