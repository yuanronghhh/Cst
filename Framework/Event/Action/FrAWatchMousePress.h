#ifndef __FR_AWATCH_MOUSE_PRESS_H__
#define __FR_AWATCH_MOUSE_PRESS_H__

#include <Framework/Event/Action/FrAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_MOUSE_PRESS (fr_awatch_mouse_press_get_type())
#define FR_AWATCH_MOUSE_PRESS(o) ((FrAWatchMousePress* )sys_object_cast_check(o, FR_TYPE_AWATCH_MOUSE_PRESS))
#define FR_AWATCH_MOUSE_PRESS_CLASS(o) ((FrAWatchMousePressClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_MOUSE_PRESS))
#define FR_AWATCH_MOUSE_PRESS_GET_CLASS(o) sys_instance_get_class(o, FrAWatchMousePressClass)

struct _FrAWatchMousePressClass {
  FrAWatchClass parent;
};

struct _FrAWatchMousePress {
  FrAWatch parent;

  /* <private> */
  FrGetBoundFunc bound_func;
};

SYS_API FrAWatchMousePress* fr_awatch_mouse_press_new(void);
SYS_API SysType fr_awatch_mouse_press_get_type(void);

void fr_awatch_mouse_press_set_bound_func(FrAWatchMousePress *self, FrGetBoundFunc bound_func);
FrGetBoundFunc fr_awatch_mouse_press_get_bound_func(FrAWatchMousePress *self);

SYS_END_DECLS

#endif
