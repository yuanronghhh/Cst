#include <Framework/Event/Action/FRAWatchKey.h>
#include <Framework/Event/Action/FRAKey.h>
#include <Framework/Event/Base/FREventKey.h>


struct _FRAWatchKeyPrivate {
  SysInt key;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRAWatchKey, fr_awatch_key, FR_TYPE_AWATCH);

static SysBool fr_awatch_key_check_i(FRAWatch *o, FREvent *e) {
  FREventKey *ekey;
  FRAWatchKey *self = FR_AWATCH_KEY(o);
  FRAWatchKeyPrivate *priv = self->priv;

  if(!fr_event_is(e, FR_TYPE_EVENT_KEY)) {
    return false;
  }

  ekey = FR_EVENT_KEY(e);

  if(priv->key == -1) {
    return true;
  }

  return priv->key == fr_event_key_key(ekey);
}

FRAWatch *fr_awatch_key_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_KEY, NULL);
}

FRAWatch *fr_awatch_key_clone_i(FRAWatch *old) {
  sys_return_val_if_fail(old != NULL, NULL);
  SysType type = sys_type_from_instance(old);
  FRAWatch *nwatch = sys_object_new(type, NULL);

  FRAWatchKeyPrivate *opriv = FR_AWATCH_KEY(old)->priv;
  FRAWatchKeyPrivate *npriv = FR_AWATCH_KEY(nwatch)->priv;

  npriv->key = opriv->key;

  return nwatch;
}

/* object api */
static void fr_awatch_key_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_awatch_key_parent_class)->dispose(o);
}

void fr_awatch_key_create_i(FRAWatch* o, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  FR_AWATCH_CLASS(fr_awatch_key_parent_class)->create(o, func_name, func, props);

  FRAWatchKey* self = FR_AWATCH_KEY(o);
  FRAWatchKeyPrivate* priv = self->priv;

  priv->key = props->key;
}

static void fr_awatch_key_class_init(FRAWatchKeyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  ocls->dispose = fr_awatch_key_dispose;

  wcls->dclone = fr_awatch_key_clone_i;
  wcls->create = fr_awatch_key_create_i;
  wcls->check = fr_awatch_key_check_i;
}

static void fr_awatch_key_init(FRAWatchKey *self) {
  self->priv = fr_awatch_key_get_private(self);

  fr_awatch_set_action(FR_AWATCH(self), FR_AKEY_STATIC);
}
