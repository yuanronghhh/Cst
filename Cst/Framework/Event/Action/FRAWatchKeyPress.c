#include <Framework/Event/Action/FRAWatchKeyPress.h>
#include <Framework/Event/Action/FRAKey.h>
#include <Framework/Event/Base/FREventKey.h>


SYS_DEFINE_TYPE(FRAWatchKeyPress, fr_awatch_key_press, FR_TYPE_AWATCH);

static SysBool fr_awatch_key_press_check_i(FRAWatch *o, FREvent *e) {
  FREventKey *ekey;
  FRAWatchKeyPress *self = FR_AWATCH_KEY_PRESS(o);

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

FRAWatch *fr_awatch_key_press_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_KEY_PRESS, NULL);
}

SysObject *fr_awatch_key_press_clone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysObject *n = SYS_OBJECT_CLASS(fr_awatch_key_press_parent_class)->dclone(o);

  FRAWatchKeyPress *oself = FR_AWATCH_KEY_PRESS(o);
  FRAWatchKeyPress *nself = FR_AWATCH_KEY_PRESS(n);

  nself->key = oself->key;

  return n;
}

/* object api */
static void fr_awatch_key_press_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_awatch_key_press_parent_class)->dispose(o);
}

void fr_awatch_key_press_create_i(FRAWatch* o, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  FR_AWATCH_CLASS(fr_awatch_key_press_parent_class)->create(o, func_name, func, props);

  FRAWatchKeyPress* self = FR_AWATCH_KEY_PRESS(o);

  self->key = props->key;
}

static void fr_awatch_key_press_class_init(FRAWatchKeyPressClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  ocls->dispose = fr_awatch_key_press_dispose;
  ocls->dclone = fr_awatch_key_press_clone_i;

  wcls->create = fr_awatch_key_press_create_i;
  wcls->check = fr_awatch_key_press_check_i;
}

static void fr_awatch_key_press_init(FRAWatchKeyPress *self) {

  fr_awatch_set_action(FR_AWATCH(self), FR_AKEY_STATIC);
}
