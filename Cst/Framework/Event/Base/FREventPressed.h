#ifndef __FR_EVENT_PRESSED_H__
#define __FR_EVENT_PRESSED_H__

#include <Framework/Event/Base/FREvent.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT_PRESSED (fr_event_pressed_get_type())
#define FR_EVENT_PRESSED(o) ((FREventPressed* )sys_object_cast_check(o, FR_TYPE_EVENT_PRESSED))
#define FR_EVENT_PRESSED_CLASS(o) ((FREventPressedClass *)sys_class_cast_check(o, FR_TYPE_EVENT_PRESSED))
#define FR_EVENT_PRESSED_GET_CLASS(o) sys_instance_get_class(o, FREventPressedClass)

struct _FREventPressedClass {
  FREventClass parent;
};

struct _FREventPressed {
  FREvent parent;

  FREventPressedPrivate *priv;
};

SYS_API SysType fr_event_pressed_get_type(void);
SYS_API FREventPressed *fr_event_pressed_new_I(FRWindow *window);

SYS_END_DECLS

#endif
