#include <Framework/Event/Action/FrAWatchKeyPress.h>
#include <Framework/Event/Action/FrAKey.h>
#include <Framework/Event/Base/FrEventKey.h>


SYS_DEFINE_TYPE(FrAWatchKeyPress, fr_awatch_key_press, FR_TYPE_AWATCH);

static SysBool fr_awatch_key_press_check_i(FrAWatch *o, FrEvent *e) {
  FrEventKey *ekey;
  FrAWatchKeyPress *self = FR_AWATCH_KEY_PRESS(o);

  if(!fr_event_is(e, FR_TYPE_EVENT_KEY)) {
    return false;
  }

  ekey = FR_EVENT_KEY(e);

  if(self->key == -1) {
    return true;
  }

  if(self->key == fr_event_key_key(ekey)
      && fr_event_key_action(ekey) == FR_KEY_PRESS) {
    return true;
  }

  return false;
}

FrAWatch *fr_awatch_key_press_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_KEY_PRESS, NULL);
}

void fr_awatch_key_press_set_key(FrAWatchKeyPress *self, SysInt key) {
  sys_return_if_fail(self != NULL);

  self->key = key;
}

SysInt fr_awatch_key_press_get_key(FrAWatchKeyPress *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->key;
}

SysObject *fr_awatch_key_press_clone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysObject *n = SYS_OBJECT_CLASS(fr_awatch_key_press_parent_class)->dclone(o);

  FrAWatchKeyPress *oself = FR_AWATCH_KEY_PRESS(o);
  FrAWatchKeyPress *nself = FR_AWATCH_KEY_PRESS(n);

  nself->key = oself->key;

  return n;
}

/* object api */
static void fr_awatch_key_press_dispose(SysObject* o) {


}

void fr_awatch_key_press_construct_i(FrAWatch* o, FrAWatchBuilder *builder) {
  FR_AWATCH_CLASS(fr_awatch_key_press_parent_class)->construct(o, builder);

  FrAWatchKeyPress* self = FR_AWATCH_KEY_PRESS(o);

  fr_awatch_builder_build_awatch_key_press(builder, self);
}

static void fr_awatch_key_press_class_init(FrAWatchKeyPressClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  ocls->dispose = fr_awatch_key_press_dispose;
  ocls->dclone = fr_awatch_key_press_clone_i;

  wcls->construct = fr_awatch_key_press_construct_i;
  wcls->check = fr_awatch_key_press_check_i;
}

static void fr_awatch_key_press_init(FrAWatchKeyPress *self) {

  fr_awatch_set_action(FR_AWATCH(self), FR_AKEY_STATIC);
}
