#include <Framework/Media/FrVideoDecoder.h>
#include <Framework/Media/FrPipeline.h>
#include <Framework/Media/FrMedia.h>
#include <Framework/Media/FrImageScale.h>

SYS_DEFINE_TYPE(FrVideoDecoder, fr_video_decoder, FR_TYPE_MEDIA_DECODER);

static SysInt fr_video_decoder_open_i(FrDecoder *o) {
  FrVideoDecoder* self = FR_VIDEO_DECODER(o);
  SysInt err = FR_DECODER_CLASS(fr_video_decoder_parent_class)->open(o);
  FrImageScaleContext info;

  fr_image_scale_create(&self->scale);

  info.in_width = self->parent.ctx->width;
  info.in_height = self->parent.ctx->height;
  info.out_width = self->parent.ctx->width;
  info.out_height = self->parent.ctx->height;
  info.in_pix_fmt = self->parent.ctx->pix_fmt;
  info.out_pix_fmt = AV_PIX_FMT_BGRA;
  fr_image_scale_construct(&self->scale, &info);

  return err;
}

static SysInt fr_video_decoder_decode_frame_i(FrMediaDecoder *o,
    FrMediaFrame *mframe) {
  FrVideoFrame* vframe;

  FrVideoDecoder* self = FR_VIDEO_DECODER(o);
  FrDecoder *d = FR_DECODER(o);
  FrPipeline *box = fr_decoder_get_user_data(d);

#if 0
  const SysChar* filename = PROJECT_DIR"/Assets/surface.png";
  fr_media_yuv_save_to_png(mframe->ctx, filename);
#endif

  vframe = (FrVideoFrame*)sys_object_dclone(mframe);
  fr_image_scale_video_frame(&self->scale, vframe);

  sys_async_queue_push(&box->image_queue, vframe);

  return 0;
}

void fr_video_decoder_resize(FrVideoDecoder *self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(width > 0);
  sys_return_if_fail(height > 0);

  fr_image_scale_resize_proportion(&self->scale, width, height, true);
}

FrMediaFrame* fr_video_decoder_get_frame_i (FrMediaDecoder* o) {
  FrVideoDecoder* self = FR_VIDEO_DECODER(o);

  return FR_MEDIA_FRAME(&self->frame);
}

/* object api */
FrDecoder* fr_video_decoder_new(void) {
  return sys_object_new(FR_TYPE_VIDEO_DECODER, NULL);
}

static void fr_video_decoder_dispose(SysObject* o) {
  FrVideoDecoder* self = FR_VIDEO_DECODER(o);

  sys_object_destroy(&self->scale);
  sys_object_destroy(&self->frame);

  SYS_OBJECT_CLASS(fr_video_decoder_parent_class)->dispose(o);
}

static void fr_video_decoder_class_init(FrVideoDecoderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrDecoderClass* dcls = FR_DECODER_CLASS(cls);
  FrMediaDecoderClass *mcls = FR_MEDIA_DECODER_CLASS(cls);

  dcls->open = fr_video_decoder_open_i;
  mcls->get_frame = fr_video_decoder_get_frame_i;
  mcls->decode_frame = fr_video_decoder_decode_frame_i;

  ocls->dispose = fr_video_decoder_dispose;
}

void fr_video_decoder_init(FrVideoDecoder* self) {
  fr_video_frame_create(&self->frame);
}
