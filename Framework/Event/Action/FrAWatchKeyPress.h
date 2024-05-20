#ifndef __FR_AWATCH_KEY_PRESS_H__
#define __FR_AWATCH_KEY_PRESS_H__

#include <Framework/Event/Action/FrAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_KEY_PRESS (fr_awatch_key_press_get_type())
#define FR_AWATCH_KEY_PRESS(o) ((FrAWatchKeyPress* )sys_object_cast_check(o, FR_TYPE_AWATCH_KEY_PRESS))
#define FR_AWATCH_KEY_PRESS_CLASS(o) ((FrAWatchKeyPressClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_KEY_PRESS))
#define FR_AWATCH_KEY_PRESS_GET_CLASS(o) sys_instance_get_class(o, FrAWatchKeyPressClass)

struct _FrAWatchKeyPressClass {
  FrAWatchClass parent;
};

struct _FrAWatchKeyPress {
  FrAWatch parent;

  /* <private> */
  SysInt key;
};

SYS_API SysType fr_awatch_key_press_get_type(void);
SYS_API FrAWatch *fr_awatch_key_press_new(void);

void fr_awatch_key_press_set_key(FrAWatchKeyPress *self, SysInt key);
SysInt fr_awatch_key_press_get_key(FrAWatchKeyPress *self);

SYS_END_DECLS

#endif

