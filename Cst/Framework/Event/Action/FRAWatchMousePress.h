#ifndef __FR_AWATCH_MOUSE_PRESS_H__
#define __FR_AWATCH_MOUSE_PRESS_H__

#include <Framework/Event/Action/FRAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_MOUSE_PRESS (fr_awatch_mouse_press_get_type())
#define FR_AWATCH_MOUSE_PRESS(o) ((FRAWatchMousePress* )sys_object_cast_check(o, FR_TYPE_AWATCH_MOUSE_PRESS))
#define FR_AWATCH_MOUSE_PRESS_CLASS(o) ((FRAWatchMousePressClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_MOUSE_PRESS))
#define FR_AWATCH_MOUSE_PRESS_GET_CLASS(o) sys_instance_get_class(o, FRAWatchMousePressClass)

struct _FRAWatchMousePressClass {
  FRAWatchClass parent;
};

struct _FRAWatchMousePress {
  FRAWatch parent;

  /* <private> */
  FRGetBoundFunc get_bound_func;
};

SYS_API FRAWatchMousePress* fr_awatch_mouse_press_new(void);
SYS_API SysType fr_awatch_mouse_press_get_type(void);

SYS_END_DECLS

#endif
