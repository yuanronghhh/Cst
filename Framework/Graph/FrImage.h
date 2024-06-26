#ifndef __FR_IMAGE_H__
#define __FR_IMAGE_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_IMAGE (fr_image_get_type())
#define FR_IMAGE(o) ((FrImage* )sys_object_cast_check(o, FR_TYPE_IMAGE))
#define FR_IMAGE_CLASS(o) ((FrImageClass *)sys_class_cast_check(o, FR_TYPE_IMAGE))
#define FR_IMAGE_GET_CLASS(o) sys_instance_get_class(o, FrImageClass)

#define MAX_IMAGE_PLANE 4

struct _FrImageContext {
  SysUInt8 *data;
  SysInt height;
  SysInt width;
  SysInt format;
  SysInt stride[MAX_IMAGE_PLANE];

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
  SysUInt8 *data;
  SysInt stride[MAX_IMAGE_PLANE];
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
#define fr_image_get_size(format, width, height) av_image_get_buffer_size(format, width, height, 1)

SysInt* fr_image_get_stride(FrImage *self);
SysInt fr_image_context_get_size(FrImageContext *info);
#define fr_image_context_fill_stride(info) av_image_fill_linesizes((info)->stride, (info)->format, (info)->width)

SYS_END_DECLS

#endif
