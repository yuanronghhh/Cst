#ifndef __FR_AWATCH_KEY_H__
#define __FR_AWATCH_KEY_H__

#include <Framework/Event/Action/FRAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_KEY (fr_awatch_key_get_type())
#define FR_AWATCH_KEY(o) ((FRAWatchKey* )sys_object_cast_check(o, FR_TYPE_AWATCH_KEY))
#define FR_AWATCH_KEY_CLASS(o) ((FRAWatchKeyClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_KEY))
#define FR_AWATCH_KEY_GET_CLASS(o) sys_instance_get_class(o, FRAWatchKeyClass)

struct _FRAWatchKeyClass {
  FRAWatchClass parent;
};

struct _FRAWatchKey {
  FRAWatch parent;

  FRAWatchKeyPrivate *priv;
};

SYS_API SysType fr_awatch_key_get_type(void);
SYS_API FRAWatch * fr_awatch_key_new_I(SysPointer user_data, SysInt scancode, FREventFunc func);

SYS_END_DECLS

#endif

