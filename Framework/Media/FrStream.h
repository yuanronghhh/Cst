#ifndef __FR_STREAM_H__
#define __FR_STREAM_H__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_STREAM (fr_stream_get_type())
#define FR_STREAM(o) ((FrStream* )sys_object_cast_check(o, FR_TYPE_STREAM))
#define FR_STREAM_CLASS(o) ((FrStreamClass *)sys_class_cast_check(o, FR_TYPE_STREAM))
#define FR_STREAM_GET_CLASS(o) sys_instance_get_class(o, FrStreamClass)

struct _FrStreamClass {
  SysObjectClass parent;
};

struct _FrStream {
  SysObject parent;

  /* <private> */
};

SYS_API SysType fr_stream_get_type(void);
SYS_API FrStream *fr_stream_new(void);

SYS_END_DECLS

#endif
