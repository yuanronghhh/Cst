#ifndef __FR_EVENT_KEY_H__
#define __FR_EVENT_KEY_H__

#include <Framework/Event/Base/FrEvent.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT_KEY (fr_event_key_get_type())
#define FR_EVENT_KEY(o) ((FrEventKey* )sys_object_cast_check(o, FR_TYPE_EVENT_KEY))
#define FR_EVENT_KEY_CLASS(o) ((FrEventKeyClass *)sys_class_cast_check(o, FR_TYPE_EVENT_KEY))
#define FR_EVENT_KEY_GET_CLASS(o) sys_instance_get_class(o, FrEventKeyClass)

struct _FrEventKeyClass {
  FrEventClass parent;
};

struct _FrEventKey {
  FrEvent parent;

  /* <private> */
  SysInt scancode;
  SysInt action;
  SysInt mods;
  SysInt key;
};

SYS_API SysType fr_event_key_get_type(void);
SYS_API FrEvent *fr_event_key_new_I(FrWindow *window, SysInt key, SysInt scancode, SysInt action, SysInt mods);
SYS_API SysInt fr_event_key_scancode(FrEventKey *self);
SYS_API SysInt fr_event_key_key(FrEventKey *self);
SYS_API SysInt fr_event_key_action(FrEventKey *self);

SYS_END_DECLS

#endif
