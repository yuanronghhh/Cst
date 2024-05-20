#include <Framework/Event/Action/FrAWatchKey.h>

#include <Framework/Event/Action/FrAKey.h>
#include <Framework/Event/Base/FrEventKey.h>


SYS_DEFINE_TYPE(FrAWatchKey, fr_awatch_key, FR_TYPE_AWATCH);

static SysBool fr_awatch_key_check_i(FrAWatch *o, FrEvent *e) {
  FrEventKey *ekey;
  FrAWatchKey *self = FR_AWATCH_KEY(o);

  if(!fr_event_is(e, FR_TYPE_EVENT_KEY)) {
    return false;
  }

  ekey = FR_EVENT_KEY(e);

  if(self->key == -1) {
    return true;
  }

  return self->key == fr_event_key_key(ekey);
}

FrAWatch *fr_awatch_key_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_KEY, NULL);
}

SysObject *fr_awatch_key_clone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysObject *nwatch = SYS_OBJECT_CLASS(fr_awatch_key_parent_class)->dclone(o);

  FrAWatchKey *oself = FR_AWATCH_KEY(o);
  FrAWatchKey *nself = FR_AWATCH_KEY(nwatch);

  nself->key = oself->key;

  return nwatch;
}

void fr_awatch_key_set_key(FrAWatchKey *self, SysInt key) {
  sys_return_if_fail(self != NULL);

  self->key = key;
}

SysInt fr_awatch_key_get_key(FrAWatchKey *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->key;
}

/* object api */
static void fr_awatch_key_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_awatch_key_parent_class)->dispose(o);
}

void fr_awatch_key_construct_i(FrAWatch* o, FrAWatchBuilder *builder) {
  FR_AWATCH_CLASS(fr_awatch_key_parent_class)->construct(o, builder);

  FrAWatchKey* self = FR_AWATCH_KEY(o);

  fr_awatch_builder_build_awatch_key(builder, self);
}

FrAWatch *fr_awatch_key_new_I(SysInt key, const SysChar *func_name, FrEventFunc func) {
  FrAWatch *o = fr_awatch_key_new();

  FrAWatchBuilder *builder = fr_awatch_builder_new_I(func_name, func);
  fr_awatch_builder_set_key(builder, key);
  fr_awatch_key_construct_i(o, builder);
  sys_object_unref(builder);


  return o;
}

static void fr_awatch_key_class_init(FrAWatchKeyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  ocls->dispose = fr_awatch_key_dispose;
  ocls->dclone = fr_awatch_key_clone_i;

  wcls->construct = fr_awatch_key_construct_i;
  wcls->check = fr_awatch_key_check_i;
}

static void fr_awatch_key_init(FrAWatchKey *self) {

  fr_awatch_set_action(FR_AWATCH(self), FR_AKEY_STATIC);
}
