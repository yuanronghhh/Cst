#ifndef __FR_AMOUSE_KEY_H__
#define __FR_AMOUSE_KEY_H__

#include <Framework/Event/Action/FRAction.h>

SYS_BEGIN_DECLS

#define FR_AMOUSE_KEY_STATIC (fr_amouse_key_get_static())
#define FR_TYPE_AMOUSE_KEY (fr_amouse_key_get_type())
#define FR_AMOUSE_KEY(o) ((FRAMouseKey* )sys_object_cast_check(o, FR_TYPE_AMOUSE_KEY))
#define FR_AMOUSE_KEY_CLASS(o) ((FRAMouseKeyClass *)sys_class_cast_check(o, FR_TYPE_AMOUSE_KEY))
#define FR_AMOUSE_KEY_GET_CLASS(o) sys_instance_get_class(o, FRAMouseKeyClass)

struct _FRAMouseKeyClass {
  FRActionClass parent;
};

struct _FRAMouseKey {
  FRAction parent;

  FRAMouseKeyPrivate *priv;
};

SYS_API FRAction* fr_amouse_key_get_static(void);
SYS_API SysType fr_amouse_key_get_type(void);

SYS_END_DECLS

#endif
