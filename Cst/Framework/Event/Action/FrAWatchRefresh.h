#ifndef __FR_AWATCH_REFRESH_H__
#define __FR_AWATCH_REFRESH_H__

#include <Framework/Event/Action/FrAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_REFRESH (fr_awatch_refresh_get_type())
#define FR_AWATCH_REFRESH(o) ((FrAWatchRefresh* )sys_object_cast_check(o, FR_TYPE_AWATCH_REFRESH))
#define FR_AWATCH_REFRESH_CLASS(o) ((FrAWatchRefreshClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_REFRESH))
#define FR_AWATCH_REFRESH_GET_CLASS(o) sys_instance_get_class(o, FrAWatchRefreshClass)

struct _FrAWatchRefreshClass {
  FrAWatchClass parent;
};

struct _FrAWatchRefresh {
  FrAWatch parent;

  /* <private> */
  SysInt64 last_clock;
  SysDouble rate_time;
  SysBool is_refreshing;
  SysPointer app_data;
};

SYS_API SysType fr_awatch_refresh_get_type(void);
SYS_API FrAWatch *fr_awatch_refresh_new(void);

SYS_END_DECLS

#endif

