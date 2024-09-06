#include <Framework/Media/FrHwAccel.h>
#include <Framework/Media/FrMediaDecoder.h>

SYS_DEFINE_TYPE(FrHwAccel, fr_hw_accel, SYS_TYPE_OBJECT);

/* object api */
static void fr_hw_accel_construct_i(FrHwAccel *self, FrHwAccelContext *info) {
  self->name = sys_strdup(info->name);
  self->accel_type = info->accel_type;
  self->ctx = info->ctx;
}

FrHwAccel* fr_hw_accel_new(void) {
  return sys_object_new(FR_TYPE_HW_ACCEL, NULL);
}

FrHwAccel *fr_hw_accel_new_I(FrHwAccelContext *info) {
  sys_return_val_if_fail(info != NULL, NULL);
  sys_return_val_if_fail(info->decoder != NULL, NULL);

  FrHwAccel *o = fr_hw_accel_new();

  if(info->name == NULL) {
    if(!fr_media_decoder_get_default_device(info->decoder,
          &info->name,
          &info->accel_type)) {
      goto fail;
    }
  }

  if(!fr_hw_accel_create(o, info)) {
    goto fail;
  }

  fr_hw_accel_construct_i(o, info);
  return o;

fail:
  sys_object_unref(o);
  return NULL;
}

static void fr_hw_accel_dispose(SysObject* o) {
  FrHwAccel *self = FR_HW_ACCEL(o);

  fr_hw_accel_free(self);

  if(self->name) {

    sys_clear_pointer(&self->name, sys_free);
  }

  SYS_OBJECT_CLASS(fr_hw_accel_parent_class)->dispose(o);
}

static void fr_hw_accel_class_init(FrHwAccelClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_hw_accel_dispose;
}

static void fr_hw_accel_init(FrHwAccel* self) {
}
