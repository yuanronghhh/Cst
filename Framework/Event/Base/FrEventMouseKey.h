#ifndef __FR_EVENT_MOUSEKEY_H__
#define __FR_EVENT_MOUSEKEY_H__

#include <Framework/Event/Base/FrEvent.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT_MOUSEKEY (fr_event_mousekey_get_type())
#define FR_EVENT_MOUSEKEY(o) ((FrEventMouseKey* )sys_object_cast_check(o, FR_TYPE_EVENT_MOUSEKEY))
#define FR_EVENT_MOUSEKEY_CLASS(o) ((FrEventMouseKeyClass *)sys_class_cast_check(o, FR_TYPE_EVENT_MOUSEKEY))
#define FR_EVENT_MOUSEKEY_GET_CLASS(o) sys_instance_get_class(o, FrEventMouseKeyClass)

struct _FrEventMouseKeyClass {
  FrEventClass parent;
};

struct _FrEventMouseKey {
  FrEvent parent;

  /* <private> */
  SysInt button;
  SysInt action;
  SysInt mods;
};

SYS_API SysType fr_event_mousekey_get_type(void);
SYS_API FrEvent * fr_event_mousekey_new_I(FrWindow * window, SysInt button, SysInt action, SysInt mods);
SYS_API SysInt fr_event_mousekey_key(FrEventMouseKey *self);
SYS_API SysInt fr_event_mousekey_action(FrEventMouseKey *self);

SYS_END_DECLS

#endif

