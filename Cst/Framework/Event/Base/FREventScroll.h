#ifndef __FR_EVENT_SCROLL_H__
#define __FR_EVENT_SCROLL_H__

#include <Framework/Event/Base/FREvent.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT_SCROLL (fr_event_scroll_get_type())
#define FR_EVENT_SCROLL(o) ((FREventScroll* )sys_object_cast_check(o, FR_TYPE_EVENT_SCROLL))
#define FR_EVENT_SCROLL_CLASS(o) ((FREventScrollClass *)sys_class_cast_check(o, FR_TYPE_EVENT_SCROLL))
#define FR_EVENT_SCROLL_GET_CLASS(o) sys_instance_get_class(o, FREventScrollClass)

struct _FREventScrollClass {
  FREventClass parent;
};

struct _FREventScroll {
  FREvent parent;

  FREventScrollPrivate *priv;
};

SYS_API SysType fr_event_scroll_get_type(void);
SYS_API FREventScroll *fr_event_scroll_new_I(FRWindow *window);

SYS_END_DECLS

#endif
