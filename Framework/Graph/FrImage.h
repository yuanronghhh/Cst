#ifndef __FR_IMAGE_H__
#define __FR_IMAGE_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_IMAGE (fr_image_get_type())
#define FR_IMAGE(o) ((FrImage* )sys_object_cast_check(o, FR_TYPE_IMAGE))
#define FR_IMAGE_CLASS(o) ((FrImageClass *)sys_class_cast_check(o, FR_TYPE_IMAGE))
#define FR_IMAGE_GET_CLASS(o) sys_instance_get_class(o, FrImageClass)

struct _FrImageClass {
  SysObjectClass parent;
};

struct _FrImage {
  SysObject parent;

  /* <private> */
};

SYS_API SysType fr_image_get_type(void);
SYS_API FrImage *fr_image_new(void);

SYS_API FrImage *fr_image_new_I(void);

SYS_END_DECLS

#endif
