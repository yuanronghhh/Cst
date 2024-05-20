#ifndef __FR_AWATCHKEY_H__
#define __FR_AWATCHKEY_H__

#include <Framework/Event/Base/FrEventKey.h>
#include <Framework/Event/Base/FrAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCHKEY (fr_awatchkey_get_type())
#define FR_AWATCHKEY(o) ((FrAWatchKey* )sys_object_cast_check(o, FR_TYPE_AWATCHKEY))
#define FR_AWATCHKEY_CLASS(o) ((FrAWatchKeyClass *)sys_class_cast_check(o, FR_TYPE_AWATCHKEY))
#define FR_AWATCHKEY_GET_CLASS(o) sys_instance_get_class(o, FrAWatchKeyClass)


struct _FrAWatchKeyClass {
  FrAWatchClass parent;
};


struct _FrAWatchKey {
  FrAWatch parent;

  FrAWatchKeyPrivate *priv;
};

SYS_API SysType fr_awatchkey_get_type(void);
SYS_API FrAWatch * fr_awatchkey_new_I(SysPointer user_data, FrEventFunc func);

SYS_END_DECLS

#endif

