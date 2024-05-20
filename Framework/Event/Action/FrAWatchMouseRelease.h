#ifndef __FR_AWATCH_MOUSE_RELEASE_H__
#define __FR_AWATCH_MOUSE_RELEASE_H__

#include <Framework/Event/Action/FrAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_MOUSE_RELEASE (fr_awatch_mouse_release_get_type())
#define FR_AWATCH_MOUSE_RELEASE(o) ((FrAWatchMouseRelease* )sys_object_cast_check(o, FR_TYPE_AWATCH_MOUSE_RELEASE))
#define FR_AWATCH_MOUSE_RELEASE_CLASS(o) ((FrAWatchMouseReleaseClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_MOUSE_RELEASE))
#define FR_AWATCH_MOUSE_RELEASE_GET_CLASS(o) sys_instance_get_class(o, FrAWatchMouseReleaseClass)

struct _FrAWatchMouseReleaseClass {
  FrAWatchClass parent;
};

struct _FrAWatchMouseRelease {
  FrAWatch parent;

  /* <private> */
  FrGetBoundFunc bound_func;
};

SYS_API FrAWatchMouseRelease* fr_awatch_mouse_release_new(void);
SYS_API SysType fr_awatch_mouse_release_get_type(void);

void fr_awatch_mouse_release_set_bound_func(FrAWatchMouseRelease *self, FrGetBoundFunc bound_func);
FrGetBoundFunc fr_awatch_mouse_release_get_bound_func(FrAWatchMouseRelease *self);

SYS_END_DECLS

#endif
