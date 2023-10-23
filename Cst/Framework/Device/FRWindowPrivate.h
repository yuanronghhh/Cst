#ifndef __FR_WINDOW_PRIVATE_H__
#define __FR_WINDOW_PRIVATE_H__

#include <Framework/Device/FRWindow.h>

SYS_BEGIN_DECLS

struct _FRWindowPrivate {
  GLFWwindow *gwindow;
  SysPointer user_data;
  FRDisplay *display;

  SysChar *title;
  FRWindow *share;
};

SYS_END_DECLS

#endif

