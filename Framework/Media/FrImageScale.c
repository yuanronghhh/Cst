#include <Framework/Media/FrImageScale.h>
#include <Framework/Graph/FrImage.h>

SYS_DEFINE_TYPE(FrImageScale, fr_image_scale, SYS_TYPE_OBJECT);

SysBool image_scale_check(
    FrImageScale *self,
    SysInt src_format,
    SysInt dst_format) {

  if(src_format == self->hw_format) {

    return true;
  }

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
  if(height <= 0) { return; }
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

SysBool fr_image_scale_convert_format(
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

  err = fr_image_scale_convert(
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
    AVFrame *src,
    AVFrame *dst) {
  sys_return_val_if_fail(src != NULL, -1);
  sys_return_val_if_fail(dst != NULL, -1);
  SysInt err;

  if(!image_scale_check(self, src->format, dst->format)) {
    return -1;
  }

  if(self->hw_format > 0 && self->hw_format == src->format) {

    err = av_hwframe_transfer_data(dst, src, 0);
    if(err < 0) {

      return err;
    }
  } else {

    err =  fr_image_scale_convert(self,
        (const uint8_t *const *)src->data,
        src->linesize,
        0,
        src->height,
        dst->data,
        dst->linesize);
  }

  return err;
}

void fr_image_scale_set_hw_format(FrImageScale *self, SysInt hw_format) {
  sys_return_if_fail(self != NULL);

  self->hw_format = hw_format;
}

SysInt fr_image_scale_get_hw_format(FrImageScale *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->hw_format;
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
