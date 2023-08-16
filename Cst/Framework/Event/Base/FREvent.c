#include <Framework/Event/Base/FREvent.h>

struct _FREventPrivate {
  FRWindow *window;
  FR_EVENT_ENUM etype;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FREvent, fr_event, SYS_TYPE_OBJECT);

SysBool fr_event_is(FREvent *self, SysType ttype) {
  SysType type = sys_type_from_instance(self);

  return type == ttype;
}

FRWindow* fr_event_get_window(FREvent *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FREventPrivate* priv = self->priv;

  return priv->window;
}

/* object api */
static void fr_event_construct(FREvent *self, FRWindow *window) {
  FREventPrivate* priv = self->priv;

  priv->window = window;
}

FREvent* fr_event_new(void) {
  return sys_object_new(FR_TYPE_EVENT, NULL);
}

FREvent *fr_event_new_I(FRWindow *window) {
  FREvent *o = fr_event_new();

  fr_event_construct(o, window);

  return o;
}

static void fr_event_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(fr_event_parent_class)->dispose(o);
}

static void fr_event_class_init(FREventClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_event_construct;
  ocls->dispose = fr_event_dispose;
}

void fr_event_init(FREvent *self) {
  self->priv = fr_event_get_private(self);
}

