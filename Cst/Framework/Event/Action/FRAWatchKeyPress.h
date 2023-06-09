#ifndef __FR_AWATCH_KEY_PRESS_H__
#define __FR_AWATCH_KEY_PRESS_H__

#include <Framework/Event/Action/FRAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_KEY_PRESS (fr_awatch_key_press_get_type())
#define FR_AWATCH_KEY_PRESS(o) ((FRAWatchKeyPress* )sys_object_cast_check(o, FR_TYPE_AWATCH_KEY_PRESS))
#define FR_AWATCH_KEY_PRESS_CLASS(o) ((FRAWatchKeyPressClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_KEY_PRESS))
#define FR_AWATCH_KEY_PRESS_GET_CLASS(o) sys_instance_get_class(o, FRAWatchKeyPressClass)

struct _FRAWatchKeyPressClass {
  FRAWatchClass parent;
};

struct _FRAWatchKeyPress {
  FRAWatch parent;

  FRAWatchKeyPressPrivate *priv;
};

SYS_API SysType fr_awatch_key_press_get_type(void);
SYS_API FRAWatch * fr_awatch_key_press_new_I(SysPointer user_data, SysInt scancode, FREventFunc func);

SYS_END_DECLS

#endif

