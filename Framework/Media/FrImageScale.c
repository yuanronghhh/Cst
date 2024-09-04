#include <Framework/Media/FrImageScale.h>
#include <Framework/Media/FrHwAccel.h>
#include <Framework/Graph/FrImage.h>

SYS_DEFINE_TYPE(FrImageScale, fr_image_scale, SYS_TYPE_OBJECT);

SysBool fr_image_scale_check(
    FrImageScale *self,
    SysInt src_format,
    SysInt dst_format) {

  if(src_format != self->in_pix_fmt) {
    sys_warning_N("input format check failed: expect %d, but %d", self->in_pix_fmt, src_format );
    return false;
  }

  if(dst_format != self->out_pix_fmt) {
    sys_warning_N("output format check failed: expect %d, but %d", self->out_pix_fmt, dst_format);
    return false;
  }

  return true;
}

SysBool fr_image_scale_check_hw_accel(FrImageScale *self, SysInt src_format) {
  sys_return_val_if_fail(self != NULL, false);
  if(self->hw_accel == NULL) { return false; }

  return fr_hw_accel_get_hw_format(self->hw_accel) == src_format;
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
  if(height <= 0) { return; }
  SysInt gv;

  gv = (SysInt)fr_media_gcd(width, height);

  prop->width = width / gv;
  prop->height = height / gv;
}

void fr_image_scale_setup_scale(FrImageScale *self) {
  self->ctx = sws_getCachedContext(self->ctx,
    self->in_width, self->in_height, self->in_pix_fmt,
    self->out_width, self->out_height, self->out_pix_fmt,
    SWS_BILINEAR, NULL, NULL, NULL);
}

void fr_image_scale_set_in_pix_fmt(FrImageScale *self, SysInt in_pix_fmt) {
  sys_return_if_fail(self != NULL);
  if(self->in_pix_fmt == in_pix_fmt) { return;}

  self->in_pix_fmt = in_pix_fmt;
  fr_image_scale_setup_scale(self);
}

SysInt fr_image_scale_get_in_pix_fmt(FrImageScale *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->in_pix_fmt;
}

void fr_image_scale_resize_output(FrImageScale *self,
    SysInt width, 
    SysInt height) {
  sys_return_if_fail(self != NULL);

  self->out_width = width;
  self->out_height = height;
  fr_image_scale_setup_scale(self);
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

SysBool fr_image_scale_scale_format(
    FrImageScale *self,
    FrImage *src,
    SysInt nformat) {

  SysInt err;

  FrImageContext info = {0};
  info.format = nformat;
  info.width = src->width;
  info.height = src->height;
  info.data_size = fr_image_get_size(nformat, src->width, src->height);
  fr_image_context_fill_buffer(&info);

  err = fr_media_image_scale_scale(
      self,
      (const uint8_t * const*)src->nbuf,
      src->stride,
      0,
      src->height,
      info.nbuf,
      info.stride);
  sys_assert(err > 0);

  for(int i = 0; i < MAX_IMAGE_PLANE; i++) {
    src->nbuf[i] = info.nbuf[i];
    src->stride[i] = info.stride[i];
  }

  sys_clear_pointer(src->data, sys_free);
  src->data = info.data;
  src->data_size = info.data_size;
  src->format = nformat;

  return err == 0;
}

SysInt fr_image_scale_scale_image(
    FrImageScale *self,
    FrImage *src,
    FrImage *dst) {

  if(!fr_image_scale_check(self, src->format, dst->format)) {
    return -1;
  }

  return fr_media_image_scale_scale(self,
      (const SysUInt8 *const *)src->data,
      src->stride,
      0,
      src->height,
      (SysUInt8 *const *)dst->data,
      dst->stride);
}

/* object api */
void fr_image_scale_construct(FrImageScale *self, FrImageScaleContext *info) {
  self->in_width = info->in_width;
  self->in_height = info->in_height;
  self->out_width = info->out_width == 0 ? info->in_width : info->out_width;
  self->out_height = info->out_height == 0 ? info->in_height : info->out_height;
  self->in_pix_fmt = info->in_pix_fmt ? info->in_pix_fmt : 0;
  self->out_pix_fmt = info->out_pix_fmt ? info->out_pix_fmt : info->in_pix_fmt;
  self->hw_accel = info->hw_accel;

  calc_proportion(self->out_width, self->out_height, &self->proportion);
  fr_image_scale_setup_scale(self);
}

FrImageScale* fr_image_scale_new(void) {
  return sys_object_new(FR_TYPE_IMAGE_SCALE, NULL);
}

FrImageScale* fr_image_scale_new_I(FrImageScaleContext *info) {
  FrImageScale* o = fr_image_scale_new();

  fr_image_scale_construct(o, info);

  return o;
}

void fr_image_scale_destroy(SysObject* o) {
  FrImageScale *self = FR_IMAGE_SCALE(o);

  if (self->ctx) {

    sys_clear_pointer(&self->ctx, sws_freeContext);
  }

  if (self->hw_accel) {

    sys_clear_pointer(&self->hw_accel, _sys_object_unref);
  }
}

void fr_image_scale_dispose(SysObject* o) {
  fr_image_scale_destroy(o);

  SYS_OBJECT_CLASS(fr_image_scale_parent_class)->dispose(o);
}

static void fr_image_scale_class_init(FrImageScaleClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->destroy = fr_image_scale_destroy;
  ocls->dispose = fr_image_scale_dispose;
}

void fr_image_scale_init(FrImageScale* self) {
}
