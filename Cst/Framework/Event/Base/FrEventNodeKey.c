#include <Framework/Event/Base/FrAWatchKey.h>


struct _FrAWatchKeyPrivate {
  FrEventFunc func;
  SysPointer user_data;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FrAWatchKey, fr_awatchkey, FR_TYPE_AWATCH);


static SysBool fr_awatchkey_check_i(FrAWatch *n, FrEvent *e) {
  return true;
}

static void fr_awatchkey_dispatch_i(FrAWatch *n, FrEvent *e) {

  FrAWatchKey *self = FR_AWATCHKEY(n);
  FrAWatchKeyPrivate* priv = self->priv;

  priv->func(e, priv->user_data);
}

/* object api */
void fr_awatchkey_construct(SysObject *o, SysPointer user_data, FrEventFunc func) {
  SYS_OBJECT_CLASS(fr_awatchkey_parent_class)->construct(o, FR_TYPE_EVENT_KEY);

  FrAWatchKey *self = FR_AWATCHKEY(o);
  FrAWatchKeyPrivate* priv = self->priv;

  priv->func = func;
  priv->user_data = user_data;
}

FrAWatch* fr_awatchkey_new(void) {
  return sys_object_new(FR_TYPE_AWATCHKEY, NULL);
}

FrAWatch * fr_awatchkey_new_I(SysPointer user_data, FrEventFunc func) {
  FrAWatch *o = fr_awatchkey_new();

  fr_awatchkey_construct(o, user_data, func);

  return o;
}

static void fr_awatchkey_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_awatchkey_parent_class)->dispose(o);
}

static void fr_awatchkey_class_init(FrAWatchKeyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrAWatchClass *ncls = FR_AWATCH_CLASS(cls);

  ncls->check = fr_awatchkey_check_i;
  ncls->dispatch = fr_awatchkey_dispatch_i;

  cls->construct = fr_awatchkey_construct;
  ocls->dispose = fr_awatchkey_dispose;
}

void fr_awatchkey_init(FrAWatchKey *self) {
  FrAWatchKeyPrivate *priv = self->priv = fr_awatchkey_get_private(self);

  self->priv = fr_awatchkey_get_private(self);
}
