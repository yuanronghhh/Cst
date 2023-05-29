#ifndef __FR_AKEY_H__
#define __FR_AKEY_H__

#include <Framework/Event/Action/FRAction.h>

SYS_BEGIN_DECLS

#define FR_AKEY_STATIC (fr_akey_get_static())
#define FR_TYPE_AKEY (fr_akey_get_type())
#define FR_AKEY(o) ((FRAKey* )sys_object_cast_check(o, FR_TYPE_AKEY))
#define FR_AKEY_CLASS(o) ((FRAKeyClass *)sys_class_cast_check(o, FR_TYPE_AKEY))
#define FR_AKEY_GET_CLASS(o) sys_instance_get_class(o, FRAKeyClass)

struct _FRAKeyClass {
  FRActionClass parent;
};

struct _FRAKey {
  FRAction parent;

  FRAKeyPrivate *priv;
};

SYS_API FRAction* fr_akey_get_static(void);
SYS_API SysType fr_akey_get_type(void);

SYS_END_DECLS

#endif
