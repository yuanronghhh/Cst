#ifndef __FR_IMAGE_H__
#define __FR_IMAGE_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_IMAGE (fr_image_get_type())
#define FR_IMAGE(o) ((FrImage* )sys_object_cast_check(o, FR_TYPE_IMAGE))
#define FR_IMAGE_CLASS(o) ((FrImageClass *)sys_class_cast_check(o, FR_TYPE_IMAGE))
#define FR_IMAGE_GET_CLASS(o) sys_instance_get_class(o, FrImageClass)

struct _FrImageContext {
  SysUInt8 *data[AV_NUM_DATA_POINTERS];
  SysInt stride[AV_NUM_DATA_POINTERS];
  SysInt height;
  SysInt width;
  SysInt format;

  /* <private> */
  SysInt data_size;
};

struct _FrImageClass {
  SysObjectClass parent;
};

struct _FrImage {
  SysObject parent;

  /* <private> */
  SysInt data_size;
  SysUInt8 *data[AV_NUM_DATA_POINTERS];
  SysInt stride[AV_NUM_DATA_POINTERS];
  SysInt height;
  SysInt width;
  SysInt format;
};

SYS_API SysType fr_image_get_type(void);
SYS_API FrImage *fr_image_new(void);

SYS_API FrImage *fr_image_new_empty(FrImageContext *info);
SYS_API FrImage* fr_image_new_with_buffer(FrImageContext *info);
SYS_API FrImage* fr_image_new_from_buffer(FrImageContext *info);
SYS_API FrImage *fr_image_new_I(FrImageContext *info);
SysBool fr_image_is_empty(FrImage* self);
FrImage* fr_image_new_from_avframe(AVFrame *frame);
FrImage *fr_image_new_from_surface(FrSurface *surface);

SysInt* fr_image_get_stride(FrImage *self);
SysInt fr_image_context_get_size(FrImageContext *info);

SYS_END_DECLS

#endif
