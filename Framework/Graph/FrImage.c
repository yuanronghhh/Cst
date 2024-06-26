#include <Framework/Graph/FrImage.h>
#include <Framework/Media/FrImageScale.h>
#include <Framework/Graph/FrIDraw.h>

SYS_DEFINE_TYPE(FrImage, fr_image, SYS_TYPE_OBJECT);

SysBool fr_image_is_empty(FrImage* self) {

  return self->data_size == 0;
}

static SysBool image_check(FrImageContext *info) {
  sys_return_val_if_fail(info->format >= 0, false);
  sys_return_val_if_fail(info->width >= 0, false);
  sys_return_val_if_fail(info->height >= 0, false);

  return true;
}

FrImage* fr_image_new_from_avframe(AVFrame *frame) {
  SysInt err = 0;
  FrImageContext info = {
    .width = frame->width,
    .height = frame->height,
    .format = frame->format,
  };

  info.data_size = fr_image_context_get_size(&info);
  info.data = sys_malloc0(info.data_size);

  err = av_image_copy_to_buffer(
      info.data,
      info.data_size,
      (const uint8_t * const*)frame->data,
      frame->linesize,
      info.format,
      info.width,
      info.height,
      1);

  if(err < 0) {

    sys_free_N(info.data);
    return NULL;
  }

  return fr_image_new_from_buffer(&info);
}

FrImage *fr_image_new_from_surface(FrSurface *surface) {
  SysUInt8* data = fr_i_draw_surface_get_data(surface);

  FrImageContext info = {0};
  info.data = data;
  info.width = 800;
  info.height = 600;
  info.format = AV_PIX_FMT_BGRA;
  info.data_size = fr_image_context_get_size(&info);

  return fr_image_new_from_buffer(&info);
}

SysInt* fr_image_get_stride(FrImage *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->stride;
}

/* object api */
static void fr_image_construct_i(FrImage *self, FrImageContext *info) {
  av_image_fill_linesizes(self->stride, info->format, info->width);

  self->height = info->height;
  self->width = info->width;
  self->format = info->format;
  self->data_size = info->data_size;

  if (info->data_size > 0) {
    self->data = info->data;
  }
}

SysInt fr_image_context_get_size(FrImageContext *info) {
  sys_return_val_if_fail(info != NULL, -1);
  sys_return_val_if_fail(info->width != 0, -1);
  sys_return_val_if_fail(info->height != 0, -1);

  return fr_image_get_size(
      info->format,
      info->width,
      info->height);
}

FrImage* fr_image_new_with_buffer(FrImageContext *info) {
  sys_return_val_if_fail(info != NULL, NULL);

  info->data = sgc_type_new(SYS_TYPE_CHAR, info->data_size);
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

  if(!image_check(info)){ return NULL; }

  fr_image_construct_i(o, info);

  return o;
}

static void fr_image_dispose(SysObject* o) {
  FrImage *self = FR_IMAGE(o);

  if(self->data_size > 0) {

    sys_clear_pointer(&self->data, sgc_free);
  }

  SYS_OBJECT_CLASS(fr_image_parent_class)->dispose(o);
}

static void fr_image_class_init(FrImageClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_image_dispose;
}

void fr_image_init(FrImage* self) {
}
