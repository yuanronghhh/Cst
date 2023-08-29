#ifndef __FR_EVENT_H__
#define __FR_EVENT_H__

#include <Framework/FRCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT (fr_event_get_type())
#define FR_EVENT(o) ((FREvent* )sys_object_cast_check(o, FR_TYPE_EVENT))
#define FR_EVENT_CLASS(o) ((FREventClass *)sys_class_cast_check(o, FR_TYPE_EVENT))
#define FR_EVENT_GET_CLASS(o) sys_instance_get_class(o, FREventClass)

struct _FREventClass {
  SysObjectClass parent;

  void (*construct)(FREvent *self, FRWindow *window);
};

struct _FREvent {
  SysObject parent;

  /* < private > */
  FRWindow *window;
  FR_EVENT_ENUM etype;
};

SYS_API SysType fr_event_get_type(void);
SYS_API FREvent *fr_event_new_I(FRWindow *window);
SYS_API FRWindow* fr_event_get_window(FREvent *self);
SYS_API SysBool fr_event_is(FREvent *self, SysType type);


SYS_END_DECLS

#endif
