#ifndef __FR_ACTION_H__
#define __FR_ACTION_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_ACTION_STATIC (fr_action_get_static())
#define FR_TYPE_ACTION (fr_action_get_type())
#define FR_ACTION(o) ((FrAction* )sys_object_cast_check(o, FR_TYPE_ACTION))
#define FR_ACTION_CLASS(o) ((FrActionClass *)sys_class_cast_check(o, FR_TYPE_ACTION))
#define FR_ACTION_GET_CLASS(o) sys_instance_get_class(o, FrActionClass)

struct _FrActionClass {
  SysObjectClass parent;

  void (*create) (FrAction *self);
  SysBool (*check) (FrAction *self, FrEvent *e);
  void (*dispatch) (FrAction *self, FrEvent *e);
};

struct _FrAction {
  SysObject parent;

  /* <private> */
  SysList *awatch_list;
  SysChar *name;
};

SYS_API FrAction* fr_action_new_I(void);
SYS_API SysType fr_action_get_type(void);
SYS_API FrAction* fr_action_get_static(void);

SYS_API void fr_action_dispatch(FrAction *self, FrEvent *e);
SYS_API SysBool fr_action_check(FrAction *self, FrEvent *e);
SYS_API SysList* fr_action_bind_awatch(FrAction *self, FrAWatch* awatch);
SYS_API void fr_action_unbind_awatch(FrAction *self, SysList *action_link);
SYS_API const SysChar *fr_action_get_name(FrAction *self);
SYS_API void fr_action_set_name(FrAction *self, const SysChar *name);

SYS_END_DECLS

#endif

