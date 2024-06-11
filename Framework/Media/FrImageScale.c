#include <Framework/Media/FrImageScale.h>
#include <Framework/Graph/FrImage.h>

SYS_DEFINE_TYPE(FrImageScale, fr_image_scale, SYS_TYPE_OBJECT);

SysBool image_scale_check(
    FrImageScale *self,
    SysInt src_format,
    SysInt dst_format) {

  if(src_format != self->in_pix_fmt) {
    sys_warning_N("input format check failed: %d -> %d", src_format, self->in_pix_fmt);
    return false;
  }

  if(dst_format != self->out_pix_fmt) {
    sys_warning_N("output format check failed: %d -> %d", dst_format, self->out_pix_fmt);
    return false;
  }

  return true;
}

void fr_image_scale_set_proportion(FrImageScale* self, FrProportion* prop) {
  sys_return_if_fail(self != NULL);

  self->proportion.width = prop->width;
  self->proportion.height = prop->height;
}

static void calc_proportion(
  SysInt width,
  SysInt height, 
  FrProportion *prop) {
  SysInt gv;

  gv = (SysInt)av_gcd(width, height);

  prop->width = width / gv;
  prop->height = height / gv;
}

static SysBool fr_image_scale_destroy_i(SysObject* o) {
  FrImageScale *self = FR_IMAGE_SCALE(o);

  if (self->ctx) {

    sys_clear_pointer(&self->ctx, sws_freeContext);
  }

  return true;
}

static void setup_scale(FrImageScale *self) {
  self->ctx = sws_getCachedContext(self->ctx,
    self->in_width, self->in_height, self->in_pix_fmt,
    self->out_width, self->out_height, self->out_pix_fmt,
    SWS_BILINEAR, NULL, NULL, NULL);
}

void fr_image_scale_resize_output(FrImageScale *self,
    SysInt width, 
    SysInt height) {
  sys_return_if_fail(self != NULL);

  self->out_width = width;
  self->out_height = height;
  setup_scale(self);
}

void fr_image_scale_resize_proportion(FrImageScale *self,
    SysInt width, 
    SysInt height,
    SysBool prop_width) {
  sys_return_if_fail(self != NULL);
  SysInt nheight, nwidth;

  if(prop_width) {

    nheight = width / self->proportion.width * self->proportion.height;
    nwidth = width;
  } else {

    nwidth = height / self->proportion.height * self->proportion.width;
    nheight = height;
  }

  fr_image_scale_resize_output(self, nwidth, nheight);
}

SysInt fr_image_scale_convert(
    FrImageScale *self,
    const SysUInt8 *const src_data[],
    const SysInt src_stride[],
    SysInt src_y,
    SysInt src_h,
    SysUInt8 *const dst_data[],
    const SysInt dst_stride[]) {

  return sws_scale(self->ctx,
    src_data,
    src_stride,
    0,
    src_h,
    dst_data,
    dst_stride);
}

SysInt fr_image_scale_convert_image(
    FrImageScale *self,
    FrImage *src,
    FrImage *dst) {

  if(!image_scale_check(self, src->format, dst->format)) {
    return -1;
  }

  return fr_image_scale_convert(self,
      (const SysUInt8 *const *)src->data,
      src->stride,
      0,
      src->height,
      (SysUInt8 *const *)dst->data,
      dst->stride);
}

SysInt fr_image_scale_convert_avframe(
    FrImageScale *self,
    AVFrame *src, AVFrame *dst) {
  sys_return_val_if_fail(src != NULL, -1);
  sys_return_val_if_fail(dst != NULL, -1);

  if(!image_scale_check(self, src->format, dst->format)) {
    return -1;
  }

  return fr_image_scale_convert(self,
      (const uint8_t *const *)src->data,
      src->linesize,
      0,
      src->height,
      dst->data,
      dst->linesize);
}


/* object api */
void fr_image_scale_construct(FrImageScale *self, FrImageScaleContext *info) {
  self->in_width = info->in_width;
  self->in_height = info->in_height;
  self->out_width = info->out_width == 0 ? info->in_width : info->out_width;
  self->out_height = info->out_height == 0 ? info->in_height : info->out_height;
  self->in_pix_fmt = info->in_pix_fmt ? info->in_pix_fmt : 0;
  self->out_pix_fmt = info->out_pix_fmt ? info->out_pix_fmt : info->in_pix_fmt;

  calc_proportion(self->out_width, self->out_height, &self->proportion);
  setup_scale(self);
}

FrImageScale* fr_image_scale_new(void) {
  return sys_object_new(FR_TYPE_IMAGE_SCALE, NULL);
}

FrImageScale* fr_image_scale_new_I(FrImageScaleContext *info) {
  FrImageScale* o = fr_image_scale_new();

  fr_image_scale_construct(o, info);

  return o;
}

static void fr_image_scale_dispose(SysObject* o) {
  FrImageScale *self = FR_IMAGE_SCALE(o);

  if (self->ctx) {

    sys_clear_pointer(&self->ctx, sws_freeContext);
  }

  SYS_OBJECT_CLASS(fr_image_scale_parent_class)->dispose(o);
}

static void fr_image_scale_class_init(FrImageScaleClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_image_scale_dispose;
  ocls->destroy = fr_image_scale_destroy_i;
}

void fr_image_scale_init(FrImageScale* self) {
}
