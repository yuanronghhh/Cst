#ifndef __FR_AWATCH_REFRESH_H__
#define __FR_AWATCH_REFRESH_H__

#include <Framework/Event/Action/FRAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_REFRESH (fr_awatch_refresh_get_type())
#define FR_AWATCH_REFRESH(o) ((FRAWatchRefresh* )sys_object_cast_check(o, FR_TYPE_AWATCH_REFRESH))
#define FR_AWATCH_REFRESH_CLASS(o) ((FRAWatchRefreshClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_REFRESH))
#define FR_AWATCH_REFRESH_GET_CLASS(o) sys_instance_get_class(o, FRAWatchRefreshClass)

struct _FRAWatchRefreshClass {
  FRAWatchClass parent;
};

struct _FRAWatchRefresh {
  FRAWatch parent;

  FRAWatchRefreshPrivate *priv;
};

SYS_API SysType fr_awatch_refresh_get_type(void);
SYS_API FRAWatch *fr_awatch_refresh_new(void);

SYS_END_DECLS

#endif

