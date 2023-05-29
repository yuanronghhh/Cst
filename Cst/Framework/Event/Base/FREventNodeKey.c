#include <Framework/Event/Base/FRAWatchKey.h>


struct _FRAWatchKeyPrivate {
  FREventFunc func;
  SysPointer user_data;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRAWatchKey, fr_awatchkey, FR_TYPE_AWATCH);


static SysBool fr_awatchkey_check_i(FRAWatch *n, FREvent *e) {
  return true;
}

static void fr_awatchkey_dispatch_i(FRAWatch *n, FREvent *e) {

  FRAWatchKey *self = FR_AWATCHKEY(n);
  FRAWatchKeyPrivate* priv = self->priv;

  priv->func(e, priv->user_data);
}

/* object api */
void fr_awatchkey_construct(SysObject *o, SysPointer user_data, FREventFunc func) {
  SYS_OBJECT_CLASS(fr_awatchkey_parent_class)->construct(o, FR_TYPE_EVENT_KEY);

  FRAWatchKey *self = FR_AWATCHKEY(o);
  FRAWatchKeyPrivate* priv = self->priv;

  priv->func = func;
  priv->user_data = user_data;
}

FRAWatch* fr_awatchkey_new(void) {
  return sys_object_new(FR_TYPE_AWATCHKEY, NULL);
}

FRAWatch * fr_awatchkey_new_I(SysPointer user_data, FREventFunc func) {
  FRAWatch *o = fr_awatchkey_new();

  fr_awatchkey_construct(SYS_OBJECT(o), user_data, func);

  return o;
}

static void fr_awatchkey_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_awatchkey_parent_class)->dispose(o);
}

static void fr_awatchkey_class_init(FRAWatchKeyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRAWatchClass *ncls = FR_AWATCH_CLASS(cls);

  ncls->check = fr_awatchkey_check_i;
  ncls->dispatch = fr_awatchkey_dispatch_i;

  ocls->construct = (SysObjectFunc)fr_awatchkey_construct;
  ocls->dispose = fr_awatchkey_dispose;
}

void fr_awatchkey_init(FRAWatchKey *self) {
  FRAWatchKeyPrivate *priv = self->priv = fr_awatchkey_get_private(self);

  self->priv = fr_awatchkey_get_private(self);
}
