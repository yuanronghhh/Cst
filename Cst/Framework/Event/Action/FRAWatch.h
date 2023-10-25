#ifndef __FR_AWATCH_H__
#define __FR_AWATCH_H__

#include <Framework/Event/Action/FRAction.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH (fr_awatch_get_type())
#define FR_AWATCH(o) ((FRAWatch* )sys_object_cast_check(o, FR_TYPE_AWATCH))
#define FR_AWATCH_CLASS(o) ((FRAWatchClass *)sys_class_cast_check(o, FR_TYPE_AWATCH))
#define FR_AWATCH_GET_CLASS(o) sys_instance_get_class(o, FRAWatchClass)

typedef const FRRect* (*FRGetBoundFunc) (SysPointer user_data);

struct _FRAWatchProps {
  /**
   * for event any type.
   */
  SysType etype;

  /**
   * for event key type .
   */
  SysInt key;

  /**
   * for mouse event and node bound see: cst_node_get_bound_bp function 
   */
  FRGetBoundFunc get_bound_func;

  /**
   * application data for refresh event.
   **/
  SysPointer app_data;
};

struct _FRAWatch {
  SysObject parent;

  /* <private> */
  FRAction *action;
  FREventFunc func;
  SysChar *func_name;
  SysPointer user_data;
  SysList *action_link;
};

struct _FRAWatchClass {
  SysObjectClass parent;

  void  (*construct) (FRAWatch* self, FRAWatchBuilder *builder);
  void  (*create) (FRAWatch* self, const SysChar *func_name, FREventFunc func, FRAWatchProps *props);
  SysBool (*check) (FRAWatch *self, FREvent *e);
  void (*dispatch) (FRAWatch *self, FREvent *e);
};

SYS_API FRAWatch* fr_awatch_new(void);
SYS_API SysType fr_awatch_get_type(void);

SYS_API SysType fr_awatch_get_type_by_name(const SysChar * name);
SYS_API void fr_awatch_dispatch(FRAWatch *self, FREvent *e);
SYS_API SysBool fr_awatch_check(FRAWatch *self, FREvent *e);

SYS_API void fr_awatch_set_action(FRAWatch *self, FRAction *action);
SYS_API FRAction *fr_awatch_get_action(FRAWatch *awatch);
SYS_API void fr_awatch_bind(FRAWatch *self, SysPointer user_data);
SYS_API void fr_awatch_unbind(FRAWatch *self);
SYS_API SysPointer fr_awatch_get_data(FRAWatch *self);
SYS_API FRAWatch *fr_awatch_list_get_awatch(SysList *list, SysType atype, const SysChar *bind_var);

SYS_API void fr_awatch_setup(void);
SYS_API void fr_awatch_teardown(void);

void fr_awatch_set_func(FRAWatch *self, FREventFunc func);
FREventFunc fr_awatch_get_func(FRAWatch *self);

SYS_END_DECLS

#endif

