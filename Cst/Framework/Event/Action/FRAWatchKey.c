#include <Framework/Event/Action/FRAWatchKey.h>
#include <Framework/Event/Action/FRAKey.h>
#include <Framework/Event/Base/FREventKey.h>


SYS_DEFINE_TYPE(FRAWatchKey, fr_awatch_key, FR_TYPE_AWATCH);

static SysBool fr_awatch_key_check_i(FRAWatch *o, FREvent *e) {
  FREventKey *ekey;
  FRAWatchKey *self = FR_AWATCH_KEY(o);

  if(!fr_event_is(e, FR_TYPE_EVENT_KEY)) {
    return false;
  }

  ekey = FR_EVENT_KEY(e);

  if(self->key == -1) {
    return true;
  }

  return self->key == fr_event_key_key(ekey);
}

FRAWatch *fr_awatch_key_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_KEY, NULL);
}

SysObject *fr_awatch_key_clone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysObject *nwatch = SYS_OBJECT_CLASS(fr_awatch_key_parent_class)->dclone(o);

  FRAWatchKey *oself = FR_AWATCH_KEY(o);
  FRAWatchKey *nself = FR_AWATCH_KEY(nwatch);

  nself->key = oself->key;

  return nwatch;
}

/* object api */
static void fr_awatch_key_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_awatch_key_parent_class)->dispose(o);
}

void fr_awatch_key_create_i(FRAWatch* o, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  FR_AWATCH_CLASS(fr_awatch_key_parent_class)->create(o, func_name, func, props);

  FRAWatchKey* self = FR_AWATCH_KEY(o);

  self->key = props->key;
}

static void fr_awatch_key_class_init(FRAWatchKeyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  ocls->dispose = fr_awatch_key_dispose;
  ocls->dclone = fr_awatch_key_clone_i;

  wcls->create = fr_awatch_key_create_i;
  wcls->check = fr_awatch_key_check_i;
}

static void fr_awatch_key_init(FRAWatchKey *self) {

  fr_awatch_set_action(FR_AWATCH(self), FR_AKEY_STATIC);
}
