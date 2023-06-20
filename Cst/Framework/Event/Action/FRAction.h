#ifndef __FR_ACTION_H__
#define __FR_ACTION_H__

#include <Framework/FRCommon.h>

SYS_BEGIN_DECLS

#define FR_ACTION_STATIC (fr_action_get_static())
#define FR_TYPE_ACTION (fr_action_get_type())
#define FR_ACTION(o) ((FRAction* )sys_object_cast_check(o, FR_TYPE_ACTION))
#define FR_ACTION_CLASS(o) ((FRActionClass *)sys_class_cast_check(o, FR_TYPE_ACTION))
#define FR_ACTION_GET_CLASS(o) sys_instance_get_class(o, FRActionClass)

struct _FRActionClass {
  SysObjectClass parent;

  void (*create) (FRAction *self);
  SysBool (*check) (FRAction *self, FREvent *e);
  void (*dispatch) (FRAction *self, FREvent *e);
};

struct _FRAction {
  SysObject parent;

  FRActionPrivate *priv;
};

SYS_API FRAction* fr_action_new_I(void);
SYS_API SysType fr_action_get_type(void);
SYS_API FRAction* fr_action_get_static(void);

SYS_API void fr_action_dispatch(FRAction *self, FREvent *e);
SYS_API SysBool fr_action_check(FRAction *self, FREvent *e);
SYS_API SysList* fr_action_bind_awatch(FRAction *self, FRAWatch* awatch);
SYS_API FRAction * fr_action_ref(FRAction * self);
SYS_API void fr_action_unbind_awatch(FRAction *self, SysList *action_link);
SYS_API const SysChar *fr_action_get_name(FRAction *self);
SYS_API void fr_action_set_name(FRAction *self, const SysChar *name);

SYS_END_DECLS

#endif

