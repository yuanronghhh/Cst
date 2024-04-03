#ifndef __FR_GLFW_H__
#define __FR_GLFW_H__

#include <System/SysCore.h>

#ifdef SYS_OS_UNIX
  #define GLFW_EXPOSE_NATIVE_X11 1
#elif defined(SYS_OS_WIN32)
  #define GLFW_EXPOSE_NATIVE_WIN32
  #define GLFW_EXPOSE_NATIVE_WGL 1
#endif

#include <glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

SYS_BEGIN_DECLS

typedef struct GLFWmonitor FRMonitor;

SYS_END_DECLS

#endif
