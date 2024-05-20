#ifndef __FR_MEDIA_FILTER_H__
#define __FR_MEDIA_FILTER_H__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_MEDIA_FILTER (fr_media_filter_get_type())
#define FR_MEDIA_FILTER(o) ((FrMediaFilter* )sys_object_cast_check(o, FR_TYPE_MEDIA_FILTER))
#define FR_MEDIA_FILTER_CLASS(o) ((FrMediaFilterClass *)sys_class_cast_check(o, FR_TYPE_MEDIA_FILTER))
#define FR_MEDIA_FILTER_GET_CLASS(o) sys_instance_get_class(o, FrMediaFilterClass)

struct _FrMediaFilterClass {
  SysObjectClass parent;
};

struct _FrMediaFilter {
  SysObject parent;

  /* <private> */
  AVFilter* ctx;
};

SYS_API SysType fr_media_filter_get_type(void);
SYS_API FrMediaFilter *fr_media_filter_new(void);

SYS_API FrMediaFilter* fr_media_filter_new_I(AVFilter* ctx);

SYS_END_DECLS

#endif
