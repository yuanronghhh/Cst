#ifndef __FR_EVENT_REFRESH_H__
#define __FR_EVENT_REFRESH_H__

#include <Framework/Event/Base/FREvent.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT_REFRESH (fr_event_refresh_get_type())
#define FR_EVENT_REFRESH(o) ((FREventRefresh* )sys_object_cast_check(o, FR_TYPE_EVENT_REFRESH))
#define FR_EVENT_REFRESH_CLASS(o) ((FREventRefreshClass *)sys_class_cast_check(o, FR_TYPE_EVENT_REFRESH))
#define FR_EVENT_REFRESH_GET_CLASS(o) sys_instance_get_class(o, FREventRefreshClass)

struct _FREventRefreshClass {
  FREventClass parent;
};

struct _FREventRefresh {
  FREvent parent;

  FREventRefreshPrivate *priv;
};

SYS_API SysType fr_event_refresh_get_type(void);
SYS_API FREvent *fr_event_refresh_new_I(FRWindow *window);


SYS_END_DECLS

#endif
