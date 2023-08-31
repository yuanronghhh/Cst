#ifndef __FR_EVENT_ANY_H__
#define __FR_EVENT_ANY_H__

#include <Framework/Event/Base/FREvent.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT_ANY (fr_event_any_get_type())
#define FR_EVENT_ANY(o) ((FREventAny* )sys_object_cast_check(o, FR_TYPE_EVENT_ANY))
#define FR_EVENT_ANY_CLASS(o) ((FREventAnyClass *)sys_class_cast_check(o, FR_TYPE_EVENT_ANY))
#define FR_EVENT_ANY_GET_CLASS(o) sys_instance_get_class(o, FREventAnyClass)

struct _FREventAnyClass {
  FREventClass parent;

  void (*construct) (FREvent *self, FRWindow *window, SysInt etype);
};

struct _FREventAny {
  FREvent parent;

  /* < private > */
  FRWindow *window;
  FR_EVENT_ENUM etype;
};

SYS_API SysType fr_event_any_get_type(void);
SYS_API FREvent *fr_event_any_new_I(FRWindow *window, SysInt etype);
SYS_API FRWindow* fr_event_any_get_window(FREventAny *self);
SYS_API SysBool fr_event_any_is(FREventAny *self, SysType type);


SYS_END_DECLS

#endif
