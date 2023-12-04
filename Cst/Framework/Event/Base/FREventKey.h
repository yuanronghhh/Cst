#ifndef __FR_EVENT_KEY_H__
#define __FR_EVENT_KEY_H__

#include <Framework/Event/Base/FREvent.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT_KEY (fr_event_key_get_type())
#define FR_EVENT_KEY(o) ((FREventKey* )sys_object_cast_check(o, FR_TYPE_EVENT_KEY))
#define FR_EVENT_KEY_CLASS(o) ((FREventKeyClass *)sys_class_cast_check(o, FR_TYPE_EVENT_KEY))
#define FR_EVENT_KEY_GET_CLASS(o) sys_instance_get_class(o, FREventKeyClass)

struct _FREventKeyClass {
  FREventClass parent;
};

struct _FREventKey {
  FREvent parent;

  /* <private> */
  SysInt scancode;
  SysInt action;
  SysInt mods;
  SysInt key;
};

SYS_API SysType fr_event_key_get_type(void);
SYS_API FREvent *fr_event_key_new_I(FRWindow *window, SysInt key, SysInt scancode, SysInt action, SysInt mods);
SYS_API SysInt fr_event_key_scancode(FREventKey *self);
SYS_API SysInt fr_event_key_key(FREventKey *self);
SYS_API SysInt fr_event_key_action(FREventKey *self);

SYS_END_DECLS

#endif
