#ifndef __FR_AWATCH_KEY_H__
#define __FR_AWATCH_KEY_H__

#include <Framework/Event/Action/FrAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_KEY (fr_awatch_key_get_type())
#define FR_AWATCH_KEY(o) ((FrAWatchKey* )sys_object_cast_check(o, FR_TYPE_AWATCH_KEY))
#define FR_AWATCH_KEY_CLASS(o) ((FrAWatchKeyClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_KEY))
#define FR_AWATCH_KEY_GET_CLASS(o) sys_instance_get_class(o, FrAWatchKeyClass)

struct _FrAWatchKeyClass {
  FrAWatchClass parent;
};

struct _FrAWatchKey {
  FrAWatch parent;

  /* <private> */
  SysInt key;
};

SYS_API SysType fr_awatch_key_get_type(void);
SYS_API FrAWatch * fr_awatch_key_new(void);
SYS_API FrAWatch *fr_awatch_key_new_I(SysInt key, const SysChar *func_name, FrEventFunc func);

void fr_awatch_key_set_key(FrAWatchKey *self, SysInt key);
SysInt fr_awatch_key_get_key(FrAWatchKey *self);

SYS_END_DECLS

#endif

