#ifndef __FR_EVENT_PRIVATE_H__
#define __FR_EVENT_PRIVATE_H__

#include <Framework/Event/Base/FrEvent.h>

SYS_BEGIN_DECLS

struct _FrEvent {
  FR_EVENT_ENUM type;
  FrWindow *window;

  SysRef ref_count;
};

struct _FrEventKey {
  FrEvent parent;

  SysInt scancode;
  SysInt action;
  SysInt mods;
  SysInt key;
};

struct _FrEventMouseKey {
  FrEvent parent;
  SysInt button;
  SysInt action;
  SysInt mods;
};

struct _FrEventCursorMove {
  FrEvent parent;

  SysDouble x;
  SysDouble y;
};

struct _FrEventScroll {
  FrEvent parent;
  SysDouble xoffset;
  SysDouble yoffset;
};

struct _FrEventPressed {
  FrEvent parent;
  SysDouble x;
  SysDouble y;
  SysPointer target;
};

SYS_END_DECLS

#endif

