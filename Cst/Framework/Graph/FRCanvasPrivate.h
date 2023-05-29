#ifndef __FR_CANVAS_PRIVATE_H__
#define __FR_CANVAS_PRIVATE_H__

#include <Framework/Graph/FRCanvas.h>

SYS_BEGIN_DECLS

struct _FRDrawContext {
  SysPointer v;

  /* not support double buffer now */
  FRContext *cr;
  FRSurface *surface;
  FRDisplay *display;
  FRWindow *window;
};


SYS_END_DECLS

#endif

