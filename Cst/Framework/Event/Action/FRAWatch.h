#ifndef __FR_AWATCH_H__
#define __FR_AWATCH_H__

#include <Framework/FRCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH (fr_awatch_get_type())
#define FR_AWATCH(o) ((FRAWatch* )sys_object_cast_check(o, FR_TYPE_AWATCH))
#define FR_AWATCH_CLASS(o) ((FRAWatchClass *)sys_class_cast_check(o, FR_TYPE_AWATCH))
#define FR_AWATCH_GET_CLASS(o) sys_instance_get_class(o, FRAWatchClass)

typedef void (*FRGetBoundFunc) (SysPointer user_data, FRRect *rect);

struct _FRAWatchProps {
  SysType etype;
  SysInt key;
  FRGetBoundFunc get_bound_func;
};

struct _FRAWatch {
  SysObject parent;

  FRAWatchPrivate *priv;
};

struct _FRAWatchClass {
  SysObjectClass parent;

  FRAWatch* (*dclone)(FRAWatch* self);
  void  (*create)(FRAWatch* self, const SysChar *func_name, FREventFunc func, FRAWatchProps *props);
  SysBool (*check) (FRAWatch *self, FREvent *e);
  void (*dispatch) (FRAWatch *self, FREvent *e);
};

SYS_API FRAWatch* fr_awatch_new(void);
SYS_API SysType fr_awatch_get_type(void);

SYS_API const SysChar * fr_awatch_get_func_name(FRAWatch * self);
SYS_API void fr_awatch_set_func_name(FRAWatch * self, const SysChar *func_name);
SYS_API void fr_awatch_set_function(FRAWatch * self, FREventFunc func);
SYS_API SysType fr_awatch_get_type_by_name(const SysChar * name);
SYS_API void fr_awatch_dispatch(FRAWatch *self, FREvent *e);
SYS_API FRAWatch * fr_awatch_new_bind(SysPointer user_data, const SysChar * watch_name, const SysChar * func_name, FREventFunc func, FRAWatchProps * props);
SYS_API FRAWatch *fr_awatch_new_by_name(const SysChar *watch_name, const SysChar *func_name, FREventFunc func, FRAWatchProps *props);
SYS_API SysBool fr_awatch_check(FRAWatch *self, FREvent *e);
SYS_API FRAWatch *fr_awatch_clone(FRAWatch *old);

SYS_API void fr_awatch_set_action(FRAWatch *self, FRAction *action);
SYS_API FRAction *fr_awatch_get_action(FRAWatch *awatch);
SYS_API void fr_awatch_bind(FRAWatch *self, SysPointer user_data);
SYS_API SysPointer fr_awatch_get_data(FRAWatch *self);

SYS_API void fr_awatch_setup(void);
SYS_API void fr_awatch_teardown(void);

SYS_END_DECLS

#endif

