#ifndef __FR_EVENT_PRESSED_H__
#define __FR_EVENT_PRESSED_H__

#include <Framework/Event/Base/FrEvent.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT_PRESSED (fr_event_pressed_get_type())
#define FR_EVENT_PRESSED(o) ((FrEventPressed* )sys_object_cast_check(o, FR_TYPE_EVENT_PRESSED))
#define FR_EVENT_PRESSED_CLASS(o) ((FrEventPressedClass *)sys_class_cast_check(o, FR_TYPE_EVENT_PRESSED))
#define FR_EVENT_PRESSED_GET_CLASS(o) sys_instance_get_class(o, FrEventPressedClass)

struct _FrEventPressedClass {
  FrEventClass parent;
};

struct _FrEventPressed {
  FrEvent parent;

  FrEventPressedPrivate *priv;
};

SYS_API SysType fr_event_pressed_get_type(void);
SYS_API FrEventPressed *fr_event_pressed_new_I(FrWindow *window);

SYS_END_DECLS

#endif
