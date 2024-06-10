#include <Framework/Graph/FrImage.h>
#include <Framework/Media/FrImageScale.h>
#include <Framework/Graph/FrIDraw.h>

SYS_DEFINE_TYPE(FrImage, fr_image, SYS_TYPE_OBJECT);

SysBool fr_image_is_empty(FrImage* self) {

  return self->data[0] == 0;
}

FrImage* fr_image_new_from_avframe(AVFrame *frame) {
  FrImageContext image_info = {
    .width = frame->width,
    .height = frame->height,
    .format = frame->format,
  };
  image_info.data[0] = frame->data[0];
  *image_info.stride = *frame->linesize;

  return fr_image_new_I(&image_info);
}

FrImage *fr_image_new_from_surface(FrSurface *surface) {
  SysUInt8* data = fr_i_draw_surface_get_data(surface);
  SysInt stride = fr_i_draw_image_surface_get_stride(surface);

  FrImageContext image_info = {0};
  image_info.data[0] = data;
  image_info.width = 800;
  image_info.height = 600;
  image_info.stride[0] = stride;
  image_info.format = AV_PIX_FMT_BGRA;
  image_info.data_size = fr_image_context_get_size(&image_info);

  return fr_image_new_from_buffer(&image_info);
}

SysInt* fr_image_get_stride(FrImage *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->stride;
}

/* object api */
static void fr_image_construct_i(FrImage *self, FrImageContext *info) {
  *self->stride = *info->stride;
  self->height = info->height;
  self->width = info->width;
  self->format = info->format;
  self->data_size = info->data_size;

  if (info->data_size > 0) {

    self->data[0] = info->data[0];
  }
}

SysInt fr_image_context_get_size(FrImageContext *info) {
  sys_return_val_if_fail(info != NULL, -1);
  sys_return_val_if_fail(info->format != 0, -1);
  sys_return_val_if_fail(info->width != 0, -1);
  sys_return_val_if_fail(info->height != 0, -1);
  sys_return_val_if_fail(info->stride[0] != 0, -1);

  return av_image_get_buffer_size(
      info->format,
      info->width,
      info->height,
      info->stride[0]);
}

FrImage* fr_image_new_with_buffer(FrImageContext *info) {
  sys_return_val_if_fail(info != NULL, NULL);

  info->data[0] = sgc_type_new(SYS_TYPE_CHAR, info->data_size);
  FrImage *o = fr_image_new_I(info);

  return o;
}

FrImage* fr_image_new_from_buffer(FrImageContext *info) {
  sys_return_val_if_fail(info != NULL, NULL);
  sys_return_val_if_fail(info->data_size != 0, NULL);

  return fr_image_new_I(info);
}

FrImage* fr_image_new_empty(FrImageContext *info) {
  sys_return_val_if_fail(info != NULL, NULL);

  info->data_size = 0;
  return fr_image_new_I(info);
}

FrImage* fr_image_new(void) {
  return sys_object_new(FR_TYPE_IMAGE, NULL);
}

FrImage *fr_image_new_I(FrImageContext *info) {
  FrImage *o = fr_image_new();

  fr_image_construct_i(o, info);

  return o;
}

static void fr_image_dispose(SysObject* o) {
  FrImage *self = FR_IMAGE(o);

  if(self->data_size > 0) {

    // sgc_free(self->data[0]);
    self->data[0] = 0;
  }

  SYS_OBJECT_CLASS(fr_image_parent_class)->dispose(o);
}

static void fr_image_class_init(FrImageClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_image_dispose;
}

void fr_image_init(FrImage* self) {
}
