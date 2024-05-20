#ifndef __FR_AWATCH_ANY_H__
#define __FR_AWATCH_ANY_H__

#include <Framework/Event/Action/FrAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_ANY (fr_awatch_any_get_type())
#define FR_AWATCH_ANY(o) ((FrAWatchAny* )sys_object_cast_check(o, FR_TYPE_AWATCH_ANY))
#define FR_AWATCH_ANY_CLASS(o) ((FrAWatchAnyClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_ANY))
#define FR_AWATCH_ANY_GET_CLASS(o) sys_instance_get_class(o, FrAWatchAnyClass)

struct _FrAWatchAnyClass {
  FrAWatchClass parent;
};

struct _FrAWatchAny {
  FrAWatch parent;

  /* <private> */
  FR_EVENT_ENUM event_enum;
};

SYS_API SysType fr_awatch_any_get_type(void);
SYS_API FrAWatch *fr_awatch_any_new(void);
SYS_API FrAWatch *fr_awatch_any_new_I(SysInt event_enum, const SysChar *func_name, FrEventFunc func);

void fr_awatch_any_set_event_enum(FrAWatchAny* self, SysInt event_enum);
SysInt fr_awatch_any_get_event_enum(FrAWatchAny *self);

SYS_END_DECLS

#endif

