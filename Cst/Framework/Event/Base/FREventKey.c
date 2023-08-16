#include <Framework/Event/Base/FREventKey.h>

struct _FREventKeyPrivate {
  SysInt scancode;
  SysInt action;
  SysInt mods;
  SysInt key;
};


SYS_DEFINE_TYPE_WITH_PRIVATE(FREventKey, fr_event_key, FR_TYPE_EVENT);


SysInt fr_event_key_scancode(FREventKey *self) {
  sys_return_val_if_fail(self != NULL, -1);

  FREventKeyPrivate* priv = self->priv;

  return priv->scancode;
}

SysInt fr_event_key_key(FREventKey *self) {
  sys_return_val_if_fail(self != NULL, -1);

  FREventKeyPrivate* priv = self->priv;

  return priv->key;
}

SysInt fr_event_key_action(FREventKey *self) {
  sys_return_val_if_fail(self != NULL, -1);

  FREventKeyPrivate* priv = self->priv;

  return priv->action;
}

/* object api */
static void fr_event_key_construct(FREvent *o, FRWindow *window, SysInt key, SysInt scancode, SysInt action, SysInt mods) {
  FR_EVENT_CLASS(fr_event_key_parent_class)->construct(o, window);

  FREventKey *self = FR_EVENT_KEY(o);
  FREventKeyPrivate* priv = self->priv;

  priv->key = key;
  priv->scancode = scancode;
  priv->action = action;
  priv->mods = mods;
}

FREvent* fr_event_key_new(void) {
  return sys_object_new(FR_TYPE_EVENT_KEY, NULL);
}

FREvent *fr_event_key_new_I(FRWindow *window, SysInt key, SysInt scancode, SysInt action, SysInt mods) {
  FREvent *o = fr_event_key_new();

  fr_event_key_construct(o, window, key, scancode, action, mods);

  return o;
}

static void fr_event_key_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_event_key_parent_class)->dispose(o);
}

static void fr_event_key_class_init(FREventKeyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FREventClass *ecls = FR_EVENT_CLASS(cls);

  ocls->dispose = fr_event_key_dispose;
}

void fr_event_key_init(FREventKey *self) {
  self->priv = fr_event_key_get_private(self);
}

