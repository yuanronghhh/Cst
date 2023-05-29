#ifndef __FR_EVENT_PRIVATE_H__
#define __FR_EVENT_PRIVATE_H__

#include <Framework/Event/Base/FREvent.h>

SYS_BEGIN_DECLS

struct _FREvent {
  FR_EVENT_ENUM type;
  FRWindow *window;

  SysRef ref_count;
};

struct _FREventKey {
  FREvent parent;

  SysInt scancode;
  SysInt action;
  SysInt mods;
  SysInt key;
};

struct _FREventMouseKey {
  FREvent parent;
  SysInt button;
  SysInt action;
  SysInt mods;
};

struct _FREventCursorMove {
  FREvent parent;

  SysDouble x;
  SysDouble y;
};


struct _FREventScroll {
  FREvent parent;
  SysDouble xoffset;
  SysDouble yoffset;
};

struct _FREventPressed {
  FREvent parent;
  SysDouble x;
  SysDouble y;
  SysPointer target;
};

SYS_END_DECLS

#endif

