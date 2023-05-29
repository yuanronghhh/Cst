#ifndef __FR_AWATCHKEY_H__
#define __FR_AWATCHKEY_H__

#include <Framework/Event/Base/FREventKey.h>
#include <Framework/Event/Base/FRAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCHKEY (fr_awatchkey_get_type())
#define FR_AWATCHKEY(o) ((FRAWatchKey* )sys_object_cast_check(o, FR_TYPE_AWATCHKEY))
#define FR_AWATCHKEY_CLASS(o) ((FRAWatchKeyClass *)sys_class_cast_check(o, FR_TYPE_AWATCHKEY))
#define FR_AWATCHKEY_GET_CLASS(o) sys_instance_get_class(o, FRAWatchKeyClass)


struct _FRAWatchKeyClass {
  FRAWatchClass parent;
};


struct _FRAWatchKey {
  FRAWatch parent;

  FRAWatchKeyPrivate *priv;
};

SYS_API SysType fr_awatchkey_get_type(void);
SYS_API FRAWatch * fr_awatchkey_new_I(SysPointer user_data, FREventFunc func);

SYS_END_DECLS

#endif

