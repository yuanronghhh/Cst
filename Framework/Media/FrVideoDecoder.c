#include <Framework/Media/FrVideoDecoder.h>
#include <Framework/Media/FrMedia.h>
#include <Framework/Media/FrImageScale.h>
#include <Framework/Media/FrHwAccel.h>

SYS_DEFINE_TYPE(FrVideoDecoder, fr_video_decoder, FR_TYPE_MEDIA_DECODER);

SysBool fr_video_decoder_get_hwaccel(FrVideoDecoder *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->hwaccel_ctx != NULL;
}

static SysInt fr_video_decoder_open_i(FrDecoder *o) {
  FrVideoDecoder* self = FR_VIDEO_DECODER(o);
  SysInt err;

  fr_image_scale_create(&self->scale);

  FrImageScaleContext info = {0};

  fr_media_decoder_get_info(
      &self->parent,
      &info.in_width,
      &info.in_height,
      &info.in_pix_fmt);

  info.out_width = info.in_width;
  info.out_height = info.in_height;
  info.out_pix_fmt = AV_PIX_FMT_BGRA;

  if(self->use_hwaccel) {
    FrHwAccelContext hwinfo = {
      .name = NULL,
      .decoder = FR_MEDIA_DECODER(self),
    };

    info.hw_accel = fr_hw_accel_new_I(&hwinfo);
  }

  fr_image_scale_construct(&self->scale, &info);

  err = FR_DECODER_CLASS(fr_video_decoder_parent_class)->open(o);

  return err;
}

static SysInt fr_video_decoder_decode_frame_i(
    FrMediaDecoder *o,
    FrMediaFrame **mframe) {

  SysInt err;
  FrVideoFrame* vframe = NULL;
  FrVideoDecoder* self = FR_VIDEO_DECODER(o);
  FrMediaFrame *omframe = NULL;
  SysInt format;

  err = FR_MEDIA_DECODER_CLASS(fr_video_decoder_parent_class)
    ->decode_frame(o, (FrMediaFrame **)&omframe);
  if(err < 0) { return err; }
  vframe = FR_VIDEO_FRAME(omframe);
  format = fr_media_frame_get_format(omframe);

  if(fr_image_scale_check_hw_accel(&self->scale, format)) {
    if(!fr_media_scale_copy_gpu_frame(&self->scale, omframe)) {
      sys_warning_N("hwaccel copy failed: %s", o->parent.name);
      return -1;
    }

    format = fr_media_frame_get_format(omframe);
    fr_image_scale_set_in_pix_fmt(&self->scale, format);
  }

  if(!fr_video_frame_scale(vframe, &self->scale)) {
    return -1;
  }
  *mframe = FR_MEDIA_FRAME(vframe);

  return err;
}

void fr_video_decoder_resize(FrVideoDecoder *self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(width > 0);
  sys_return_if_fail(height > 0);

  fr_image_scale_resize_proportion(&self->scale, width, height, true);
}

void fr_video_decoder_get_size(FrVideoDecoder *self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);

  *width = self->scale.out_width;
  *height = self->scale.out_height;
}

/* object api */
FrMediaDecoder* fr_video_decoder_new(void) {
  return sys_object_new(FR_TYPE_VIDEO_DECODER, NULL);
}

static void fr_video_decoder_dispose(SysObject* o) {
  FrVideoDecoder* self = FR_VIDEO_DECODER(o);

  sys_object_destroy(&self->scale);

  SYS_OBJECT_CLASS(fr_video_decoder_parent_class)->dispose(o);
}

static void fr_video_decoder_class_init(FrVideoDecoderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrDecoderClass* dcls = FR_DECODER_CLASS(cls);
  FrMediaDecoderClass *mcls = FR_MEDIA_DECODER_CLASS(cls);

  dcls->open = fr_video_decoder_open_i;
  mcls->decode_frame = fr_video_decoder_decode_frame_i;

  ocls->dispose = fr_video_decoder_dispose;
}

void fr_video_decoder_init(FrVideoDecoder* self) {
  FrMediaDecoder *o = FR_MEDIA_DECODER(self);

  fr_media_decoder_set_frame_type(o, FR_TYPE_VIDEO_FRAME);
  self->use_hwaccel = true;
}
