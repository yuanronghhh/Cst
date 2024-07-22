#include <Framework/Media/FrHwAccel.h>
#include <Framework/Media/FrMediaDecoder.h>

SYS_DEFINE_TYPE(FrHwAccel, fr_hw_accel, SYS_TYPE_OBJECT);

static enum AVPixelFormat hw_pix_format = 0;

static enum AVPixelFormat get_hw_format(
    AVCodecContext *ctx,
    const enum AVPixelFormat *pix_fmts) {
  const enum AVPixelFormat *p;

  for (p = pix_fmts; *p != -1; p++) {
    if (*p == hw_pix_format)
      return *p;
  }

  sys_warning_N("%s", "Failed to get HW surface format.");
  return AV_PIX_FMT_NONE;
}

SysInt fr_hw_accel_get_hw_format(FrHwAccel *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return hw_pix_format;
}

/* object api */
static void fr_hw_accel_construct_i(FrHwAccel *self, FrHwAccelContext *info) {
  self->name = sys_strdup(info->name);
  self->ctx = info->ctx;
}

FrHwAccel* fr_hw_accel_new(void) {
  return sys_object_new(FR_TYPE_HW_ACCEL, NULL);
}

FrHwAccel *fr_hw_accel_new_I(FrHwAccelContext *info) {
  sys_return_val_if_fail(info != NULL, NULL);
  sys_return_val_if_fail(info->name != NULL, NULL);
  sys_return_val_if_fail(info->decoder != NULL, NULL);

  AVBufferRef *ctx = NULL;
  FrMediaDecoder *dec = info->decoder;

  SysInt type = av_hwdevice_find_type_by_name(info->name);
  if(type == 0) {

    sys_warning_N("not support hardware device %s", info->name);
    return NULL;
  }

  if(!fr_media_decoder_get_hw_info(dec, type, &hw_pix_format)) {
    sys_warning_N("Failed to get hw info: %s, %s",
        fr_decoder_get_name(FR_DECODER(dec)),
        av_hwdevice_get_type_name(type));

    return NULL;
  }

  /* TODO: copy from example  */
  dec->ctx->get_format  = get_hw_format;
  if(av_hwdevice_ctx_create(&ctx, type, NULL, NULL, 0) < 0) {

    sys_warning_N("%s", "Failed to create specified HW device.");
    return NULL;
  }
  dec->ctx->hw_device_ctx = av_buffer_ref(ctx);
  info->ctx = ctx;

  FrHwAccel *o = fr_hw_accel_new();
  fr_hw_accel_construct_i(o, info);

  return o;
}

static void fr_hw_accel_dispose(SysObject* o) {
  FrHwAccel *self = FR_HW_ACCEL(o);

  av_buffer_unref(&self->ctx);
  sys_clear_pointer(&self->name, sys_free);

  SYS_OBJECT_CLASS(fr_hw_accel_parent_class)->dispose(o);
}

static void fr_hw_accel_class_init(FrHwAccelClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_hw_accel_dispose;
}

static void fr_hw_accel_init(FrHwAccel* self) {
}
