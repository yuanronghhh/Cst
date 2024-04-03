#include <Framework/Event/Base/FrEvent.h>

SYS_DEFINE_TYPE(FrEvent, fr_event, SYS_TYPE_OBJECT);

SysBool fr_event_is(FrEvent *self, SysType ttype) {
  SysType type = sys_type_from_instance(self);

  return type == ttype;
}

FrWindow* fr_event_get_window(FrEvent *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->window;
}

/* object api */
static void fr_event_construct(FrEvent *self, FrWindow *window) {

  self->window = window;
}

FrEvent* fr_event_new(void) {
  return sys_object_new(FR_TYPE_EVENT, NULL);
}

FrEvent *fr_event_new_I(FrWindow *window) {
  FrEvent *o = fr_event_new();

  fr_event_construct(o, window);

  return o;
}

static void fr_event_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(fr_event_parent_class)->dispose(o);
}

static void fr_event_class_init(FrEventClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_event_construct;
  ocls->dispose = fr_event_dispose;
}

void fr_event_init(FrEvent *self) {
  self->window = NULL;
}

