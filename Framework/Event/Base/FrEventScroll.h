#ifndef __FR_EVENT_SCROLL_H__
#define __FR_EVENT_SCROLL_H__

#include <Framework/Event/Base/FrEvent.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT_SCROLL (fr_event_scroll_get_type())
#define FR_EVENT_SCROLL(o) ((FrEventScroll* )sys_object_cast_check(o, FR_TYPE_EVENT_SCROLL))
#define FR_EVENT_SCROLL_CLASS(o) ((FrEventScrollClass *)sys_class_cast_check(o, FR_TYPE_EVENT_SCROLL))
#define FR_EVENT_SCROLL_GET_CLASS(o) sys_instance_get_class(o, FrEventScrollClass)

struct _FrEventScrollClass {
  FrEventClass parent;
};

struct _FrEventScroll {
  FrEvent parent;

  FrEventScrollPrivate *priv;
};

SYS_API SysType fr_event_scroll_get_type(void);
SYS_API FrEventScroll *fr_event_scroll_new_I(FrWindow *window);

SYS_END_DECLS

#endif
