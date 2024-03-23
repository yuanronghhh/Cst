#ifndef __FR_EVENT_H__
#define __FR_EVENT_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT (fr_event_get_type())
#define FR_EVENT(o) ((FrEvent* )sys_object_cast_check(o, FR_TYPE_EVENT))
#define FR_EVENT_CLASS(o) ((FrEventClass *)sys_class_cast_check(o, FR_TYPE_EVENT))
#define FR_EVENT_GET_CLASS(o) sys_instance_get_class(o, FrEventClass)

struct _FrEventClass {
  SysObjectClass parent;

  void (*construct) (FrEvent *self, FrWindow *window);
};

struct _FrEvent {
  SysObject parent;

  /* < private > */
  FrWindow *window;
};

SYS_API SysType fr_event_get_type(void);
SYS_API FrEvent *fr_event_new_I(FrWindow *window);
SYS_API FrWindow* fr_event_get_window(FrEvent *self);
SYS_API SysBool fr_event_is(FrEvent *self, SysType type);


SYS_END_DECLS

#endif
