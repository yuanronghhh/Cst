#ifndef __FR_EVENT_REFRESH_H__
#define __FR_EVENT_REFRESH_H__

#include <Framework/Event/Base/FrEvent.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT_REFRESH (fr_event_refresh_get_type())
#define FR_EVENT_REFRESH(o) ((FrEventRefresh* )sys_object_cast_check(o, FR_TYPE_EVENT_REFRESH))
#define FR_EVENT_REFRESH_CLASS(o) ((FrEventRefreshClass *)sys_class_cast_check(o, FR_TYPE_EVENT_REFRESH))
#define FR_EVENT_REFRESH_GET_CLASS(o) sys_instance_get_class(o, FrEventRefreshClass)

struct _FrEventRefreshClass {
  FrEventClass parent;
};

struct _FrEventRefresh {
  FrEvent parent;

  /* <private> */
  FrBound *bound;
};

SYS_API SysType fr_event_refresh_get_type(void);
SYS_API FrEvent *fr_event_refresh_new_I(FrWindow *window);
SYS_API FrEvent *fr_event_refresh_new_bound(FrWindow *window, FrBound *bound);


SYS_END_DECLS

#endif
