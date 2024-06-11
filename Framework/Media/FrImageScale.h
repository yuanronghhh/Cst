#ifndef __FR_IMAGE_SCALE_H__
#define __FR_IMAGE_SCALE_H__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_IMAGE_SCALE (fr_image_scale_get_type())
#define FR_IMAGE_SCALE(o) ((FrImageScale* )sys_object_cast_check(o, FR_TYPE_IMAGE_SCALE))
#define FR_IMAGE_SCALE_CLASS(o) ((FrImageScaleClass *)sys_class_cast_check(o, FR_TYPE_IMAGE_SCALE))
#define FR_IMAGE_SCALE_GET_CLASS(o) sys_instance_get_class(o, FrImageScaleClass)

struct _FrImageScaleClass {
  SysObjectClass parent;
};

struct _FrProportion {
  SysInt width;
  SysInt height;
};

struct _FrImageScaleContext {
  SysInt in_width;
  SysInt in_height;
  SysInt out_width;
  SysInt out_height;
  SysInt in_pix_fmt;
  SysInt out_pix_fmt;
};

struct _FrImageScale  {
  SysObject parent;

  /* <private> */
  SysInt in_width;
  SysInt in_height;
  SysInt out_width;
  SysInt out_height;
  SysInt in_pix_fmt;
  SysInt out_pix_fmt;

  FrProportion proportion;
  SysBool scale_prop;
  struct SwsContext *ctx;
};

SYS_API SysType fr_image_scale_get_type(void);
SYS_API FrImageScale* fr_image_scale_new(void);
SYS_API FrImageScale *fr_image_scale_new_I(FrImageScaleContext *info);
SYS_API void fr_image_scale_construct(FrImageScale* o, FrImageScaleContext* info);

SYS_API void fr_image_scale_resize_proportion(FrImageScale *self,
    SysInt width,
    SysInt height,
    SysBool prop_width);

SysInt fr_image_scale_convert(
    FrImageScale *self,
    const SysUInt8 *const src_data[],
    const SysInt src_stride[],
    SysInt src_y,
    SysInt src_h,
    SysUInt8 *const dst_data[],
    const SysInt dst_stride[]);

SysInt fr_image_scale_convert_image(
    FrImageScale *self,
    FrImage *src,
    FrImage *dst);

SysInt fr_image_scale_convert_avframe(
    FrImageScale *self,
    AVFrame *frame, AVFrame *dst);

SysBool fr_image_scale_convert_format(
    FrImageScale *self,
    FrImage *src,
    SysInt nformat);

#define fr_image_scale_create(o) sys_object_create(o, FR_TYPE_IMAGE_SCALE)

SYS_END_DECLS

#endif
