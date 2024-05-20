#ifndef __FR_EVENT_ANY_H__
#define __FR_EVENT_ANY_H__

#include <Framework/Event/Base/FrEvent.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT_ANY (fr_event_any_get_type())
#define FR_EVENT_ANY(o) ((FrEventAny* )sys_object_cast_check(o, FR_TYPE_EVENT_ANY))
#define FR_EVENT_ANY_CLASS(o) ((FrEventAnyClass *)sys_class_cast_check(o, FR_TYPE_EVENT_ANY))
#define FR_EVENT_ANY_GET_CLASS(o) sys_instance_get_class(o, FrEventAnyClass)

struct _FrEventAnyClass {
  FrEventClass parent;

  void (*construct) (FrEvent *self, FrWindow *window, SysInt etype);
};

struct _FrEventAny {
  FrEvent parent;

  /* < private > */
  FrWindow *window;
  FR_EVENT_ENUM event_enum;
};

SYS_API SysType fr_event_any_get_type(void);
SYS_API FrEvent *fr_event_any_new_I(FrWindow *window, SysInt event_enum);
SYS_API FrWindow* fr_event_any_get_window(FrEventAny *self);
SYS_API SysBool fr_event_any_is(FrEventAny *self, SysType type);


SYS_END_DECLS

#endif
