#ifndef __FR_SDL2_H__
#define __FR_SDL2_H__

#include <System/SysCore.h>

SYS_BEGIN_DECLS

#if SYS_OS_UNIX
 #define SDL_VIDEO_DRIVER_X11
#elif SYS_OS_WIN32
#endif
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

SYS_END_DECLS

#endif
