#include <Framework/Media/FrVideoDecoder.h>
#include <Framework/Media/FrMediaPipeline.h>
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

  FrImageScaleContext info;
  info.in_width = self->parent.ctx->width;
  info.in_height = self->parent.ctx->height;
  info.out_width = self->parent.ctx->width;
  info.out_height = self->parent.ctx->height;
  info.in_pix_fmt = self->parent.ctx->pix_fmt;
  info.out_pix_fmt = AV_PIX_FMT_BGRA;

  if(self->hwaccel_name) {
    FrHwAccelContext hwinfo = {
      .name = self->hwaccel_name,
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

  err = FR_MEDIA_DECODER_CLASS(fr_video_decoder_parent_class)
    ->decode_frame(o, (FrMediaFrame **)&omframe);
  if(err < 0) { return err; }
  vframe = FR_VIDEO_FRAME(omframe);
  fr_media_video_frame_init(vframe);

  if(!fr_media_scale_copy_gpu_frame(&self->scale, omframe)) {
    sys_warning_N("hwaccel copy failed: %s", o->parent.name);
    return -1;
  }
  fr_image_scale_set_in_pix_fmt(&self->scale, omframe->ctx->format);

  if(!fr_video_frame_scale(vframe, &self->scale)) {
    return -1;
  }
  *mframe = FR_MEDIA_FRAME(vframe);
  vframe->delay = (SysInt)(1.0 / 30 * 1e3);

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
FrDecoder* fr_video_decoder_new(void) {
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
#if SYS_OS_WIN32
  self->hwaccel_name = "d3d11va";
#elif SYS_OS_UNIX
  self->hwaccel_name = "vaapi";
#endif
}
