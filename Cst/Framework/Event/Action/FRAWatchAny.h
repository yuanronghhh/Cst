#ifndef __FR_AWATCH_ANY_H__
#define __FR_AWATCH_ANY_H__

#include <Framework/Event/Action/FRAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_ANY (fr_awatch_any_get_type())
#define FR_AWATCH_ANY(o) ((FRAWatchAny* )sys_object_cast_check(o, FR_TYPE_AWATCH_ANY))
#define FR_AWATCH_ANY_CLASS(o) ((FRAWatchAnyClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_ANY))
#define FR_AWATCH_ANY_GET_CLASS(o) sys_instance_get_class(o, FRAWatchAnyClass)

struct _FRAWatchAnyClass {
  FRAWatchClass parent;
};

struct _FRAWatchAny {
  FRAWatch parent;

  /* <private> */
  SysType etype;
};

SYS_API SysType fr_awatch_any_get_type(void);
SYS_API FRAWatch *fr_awatch_any_new(void);
SYS_API FRAWatch *fr_awatch_any_new_I(SysType etype, const SysChar *func_name, FREventFunc func);

void fr_awatch_any_set_etype(FRAWatchAny *self, SysType etype);
SysType fr_awatch_any_get_etype(FRAWatchAny *self);

SYS_END_DECLS

#endif

