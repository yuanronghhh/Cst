#ifndef __FR_AWATCH_H__
#define __FR_AWATCH_H__


#include <Framework/Event/Action/FrAction.h>
#include <Framework/Event/Action/FrAWatchBuilder.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH (fr_awatch_get_type())
#define FR_AWATCH(o) ((FrAWatch* )sys_object_cast_check(o, FR_TYPE_AWATCH))
#define FR_AWATCH_CLASS(o) ((FrAWatchClass *)sys_class_cast_check(o, FR_TYPE_AWATCH))
#define FR_AWATCH_GET_CLASS(o) sys_instance_get_class(o, FrAWatchClass)

struct _FrAWatch {
  SysObject parent;

  /* <private> */
  FrAction *action;
  FrEventFunc func;
  SysChar *func_name;
  SysPointer user_data;
  SysList *action_link;
};

struct _FrAWatchClass {
  SysObjectClass parent;

  void  (*construct) (FrAWatch* self, FrAWatchBuilder *builder);
  SysBool (*check) (FrAWatch *self, FrEvent *e);
  void (*dispatch) (FrAWatch *self, FrEvent *e);
};

SYS_API FrAWatch* fr_awatch_new(void);
SYS_API SysType fr_awatch_get_type(void);

SYS_API SysType fr_awatch_get_type_by_name(const SysChar * name);
SYS_API FrAWatch* fr_awatch_new_by_name(const SysChar *name);
SYS_API FrAWatch *fr_awatch_new_by_type(SysType tp, const SysChar * func_name, FrEventFunc watch_func);

SYS_API void fr_awatch_construct(FrAWatch* self, FrAWatchBuilder *builder);
SYS_API void fr_awatch_dispatch(FrAWatch *self, FrEvent *e);
SYS_API SysBool fr_awatch_check(FrAWatch *self, FrEvent *e);

SYS_API void fr_awatch_set_action(FrAWatch *self, FrAction *action);
SYS_API FrAction *fr_awatch_get_action(FrAWatch *awatch);
SYS_API void fr_awatch_bind(FrAWatch *self, SysPointer user_data);
SYS_API void fr_awatch_unbind(FrAWatch *self);
SYS_API SysPointer fr_awatch_get_data(FrAWatch *self);
SYS_API FrAWatch *fr_awatch_list_get_awatch(SysList *list, SysType atype, const SysChar *bind_var);

SYS_API void fr_awatch_setup(void);
SYS_API void fr_awatch_teardown(void);

void fr_awatch_set_func(FrAWatch *self, FrEventFunc func);
FrEventFunc fr_awatch_get_func(FrAWatch *self);

void fr_awatch_set_user_data(FrAWatch *self, SysPointer user_data);
SysPointer fr_awatch_get_user_data(FrAWatch *self);

void fr_awatch_set_func_name(FrAWatch *self, const SysChar * func_name);
const SysChar * fr_awatch_get_func_name(FrAWatch *self);

SYS_END_DECLS

#endif

