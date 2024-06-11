#include <Framework/Media/FrImageSaver.h>
#include <Framework/Graph/FrImage.h>

SYS_DEFINE_TYPE(FrImageSaver, fr_image_saver, SYS_TYPE_OBJECT);

SysBool data_save_to_png(
    SysInt width,
    SysInt height,
    const SysUInt8* data,
    SysInt linesize,
    SysInt format,
    const SysChar *filename) {

  FILE *fp = sys_fopen(filename, "wb");
  if (!fp) { return false; }

  png_struct* png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png_ptr) { 
    fclose(fp);
    return false;
  }

  png_info* info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr) {
    png_destroy_write_struct(&png_ptr, NULL);
    fclose(fp);
    return false;
  }

  png_init_io(png_ptr, fp);
  png_set_IHDR(png_ptr,
      info_ptr,
      width,
      height,
      8, 
      format, // PNG_COLOR_TYPE_RGB_ALPHA
      PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_DEFAULT,
      PNG_FILTER_TYPE_DEFAULT);

  png_byte** bptr = (png_byte **)sys_malloc_N(sizeof(png_byte *) * height);
  for(int i = 0; i < height; i++) {
    bptr[i] = (png_byte *)(data + i * linesize);
  }

  png_set_rows(png_ptr, info_ptr, bptr);
  png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
  sys_free_N(bptr);

  png_destroy_write_struct(&png_ptr, &info_ptr);
  fclose(fp);
  return true;
}

SysBool fr_image_saver_save_png(
    FrImageSaver *saver,
    FrImage *image,
    const SysChar *filename) {

  sys_return_val_if_fail(saver != NULL, false);
  sys_return_val_if_fail(image != NULL, false);

  if (image->format != AV_PIX_FMT_RGBA) {

    sys_warning_N("image format should be argb: %s", filename);
    return -1;
  }

  return data_save_to_png(
      image->width,
      image->height,
      image->data,
      image->stride[0],
      PNG_COLOR_TYPE_RGB_ALPHA,
      filename);
}

SysBool fr_image_saver_save_avframe(
    FrImageSaver *saver,
    AVFrame *frame,
    const SysChar *filename) {
  sys_return_val_if_fail(saver != NULL, false);
  sys_return_val_if_fail(frame != NULL, false);

  if (frame->format != AV_PIX_FMT_RGBA) {

    sys_warning_N("frame format should be argb: %s", filename);
    return -1;
  }

  return data_save_to_png(
      frame->width,
      frame->height,
      frame->data[0],
      frame->linesize[0],
      PNG_COLOR_TYPE_RGB_ALPHA,
      filename);
}

/* object api */
static void fr_image_saver_construct_i(FrImageSaver *self) {

}

FrImageSaver* fr_image_saver_new(void) {
  return sys_object_new(FR_TYPE_IMAGE_SAVER, NULL);
}

FrImageSaver *fr_image_saver_new_I(void) {
  FrImageSaver *o = fr_image_saver_new();

  fr_image_saver_construct_i(o);

  return o;
}

static void fr_image_saver_dispose(SysObject* o) {
  FrImageSaver *self = FR_IMAGE_SAVER(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(fr_image_saver_parent_class)->dispose(o);
}

static void fr_image_saver_class_init(FrImageSaverClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_image_saver_dispose;
}

void fr_image_saver_init(FrImageSaver* self) {
}
