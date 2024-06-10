#ifndef __FR_IMAGE_SAVER_H__
#define __FR_IMAGE_SAVER_H__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_IMAGE_SAVER (fr_image_saver_get_type())
#define FR_IMAGE_SAVER(o) ((FrImageSaver* )sys_object_cast_check(o, FR_TYPE_IMAGE_SAVER))
#define FR_IMAGE_SAVER_CLASS(o) ((FrImageSaverClass *)sys_class_cast_check(o, FR_TYPE_IMAGE_SAVER))
#define FR_IMAGE_SAVER_GET_CLASS(o) sys_instance_get_class(o, FrImageSaverClass)

struct _FrImageSaverClass {
  SysObjectClass parent;
};

struct _FrImageSaver {
  SysObject parent;

  /* <private> */
};

SYS_API SysType fr_image_saver_get_type(void);
SYS_API FrImageSaver *fr_image_saver_new(void);

SYS_API FrImageSaver *fr_image_saver_new_I(void);
SYS_API SysBool fr_image_saver_save_png(
    FrImageSaver *saver,
    FrImage *image,
    const SysChar *filename);

SYS_END_DECLS

#endif
