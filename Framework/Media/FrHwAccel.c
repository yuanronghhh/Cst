#include <Framework/Media/FrHwAccel.h>

SYS_DEFINE_TYPE(FrHwAccel, fr_hw_accel, SYS_TYPE_OBJECT);

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

  SysInt err;

  SysInt type = av_hwdevice_find_type_by_name(info->name);
  if(type == 0) { return NULL; }

  AVBufferRef *ctx = av_hwdevice_ctx_alloc(type);
  if(ctx == NULL) { return NULL; }
  info->ctx = ctx;

  err = av_hwdevice_ctx_init(ctx);
  if(err != 0) { return NULL; }

  FrHwAccel *o = fr_hw_accel_new();
  fr_hw_accel_construct_i(o, info);

  return o;
}

FrHwAccel *fr_hw_accel_new_with_name(const SysChar *name) {
  FrHwAccelContext info = {.name = name};

  return fr_hw_accel_new_I(&info);
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
