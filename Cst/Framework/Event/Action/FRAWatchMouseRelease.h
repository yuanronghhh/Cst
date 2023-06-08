#ifndef __FR_AWATCH_MOUSE_RELEASE_H__
#define __FR_AWATCH_MOUSE_RELEASE_H__

#include <Framework/Event/Action/FRAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_MOUSE_RELEASE (fr_awatch_mouse_release_get_type())
#define FR_AWATCH_MOUSE_RELEASE(o) ((FRAWatchMouseRelease* )sys_object_cast_check(o, FR_TYPE_AWATCH_MOUSE_RELEASE))
#define FR_AWATCH_MOUSE_RELEASE_CLASS(o) ((FRAWatchMouseReleaseClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_MOUSE_RELEASE))
#define FR_AWATCH_MOUSE_RELEASE_GET_CLASS(o) sys_instance_get_class(o, FRAWatchMouseReleaseClass)

struct _FRAWatchMouseReleaseClass {
  FRAWatchClass parent;
};

struct _FRAWatchMouseRelease {
  FRAWatch parent;

  FRAWatchMouseReleasePrivate *priv;
};

SYS_API FRAWatchMouseRelease* fr_awatch_mouse_release_new(void);
SYS_API SysType fr_awatch_mouse_release_get_type(void);

SYS_END_DECLS

#endif
