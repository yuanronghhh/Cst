#include <Framework/Event/Base/FREventMouseKey.h>


struct _FREventMouseKeyPrivate {
  SysInt button;
  SysInt action;
  SysInt mods;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FREventMouseKey, fr_event_mousekey, FR_TYPE_EVENT);


SysInt fr_event_mousekey_key(FREventMouseKey *self) {
  sys_return_val_if_fail(self != NULL, -1);

  FREventMouseKeyPrivate* priv = self->priv;

  return priv->button;
}

SysInt fr_event_mousekey_action(FREventMouseKey *self) {
  sys_return_val_if_fail(self != NULL, -1);

  FREventMouseKeyPrivate* priv = self->priv;

  return priv->action;
}

/* object api */
static void fr_event_mousekey_construct(FREvent *o, FRWindow *window, SysInt button, SysInt action, SysInt mods) {
  FR_EVENT_CLASS(fr_event_mousekey_parent_class)->construct(o, window);

  FREventMouseKey* self = FR_EVENT_MOUSEKEY(o);
  FREventMouseKeyPrivate* priv = self->priv;

  priv->button = button;
  priv->action = action;
  priv->mods = mods;
}

FREvent* fr_event_mousekey_new(void) {
  return sys_object_new(FR_TYPE_EVENT_MOUSEKEY, NULL);
}

FREvent *fr_event_mousekey_new_I(FRWindow *window, SysInt button, SysInt action, SysInt mods) {
  FREvent *o = fr_event_mousekey_new();

  fr_event_mousekey_construct(o, window, button, action, mods);

  return o;
}

static void fr_event_mousekey_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_event_mousekey_parent_class)->dispose(o);
}

static void fr_event_mousekey_class_init(FREventMouseKeyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_event_mousekey_dispose;
}

void fr_event_mousekey_init(FREventMouseKey *self) {
  self->priv = fr_event_mousekey_get_private(self);
}

