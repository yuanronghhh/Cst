#ifndef __FR_EVENT_MOUSEKEY_H__
#define __FR_EVENT_MOUSEKEY_H__

#include <Framework/Event/Base/FREvent.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT_MOUSEKEY (fr_event_mousekey_get_type())
#define FR_EVENT_MOUSEKEY(o) ((FREventMouseKey* )sys_object_cast_check(o, FR_TYPE_EVENT_MOUSEKEY))
#define FR_EVENT_MOUSEKEY_CLASS(o) ((FREventMouseKeyClass *)sys_class_cast_check(o, FR_TYPE_EVENT_MOUSEKEY))
#define FR_EVENT_MOUSEKEY_GET_CLASS(o) sys_instance_get_class(o, FREventMouseKeyClass)

struct _FREventMouseKeyClass {
  FREventClass parent;
};

struct _FREventMouseKey {
  FREvent parent;

  FREventMouseKeyPrivate *priv;
};

SYS_API SysType fr_event_mousekey_get_type(void);
SYS_API FREvent * fr_event_mousekey_new_I(FRWindow * window, SysInt button, SysInt action, SysInt mods);
SYS_API SysInt fr_event_mousekey_key(FREventMouseKey *self);
SYS_API SysInt fr_event_mousekey_action(FREventMouseKey *self);

SYS_END_DECLS

#endif

