#include <Framework/Event/Base/FREventAny.h>


SYS_DEFINE_TYPE(FREventAny, fr_event_any, FR_TYPE_EVENT);

SysBool fr_event_any_is(FREventAny *self, SysType ttype) {
  SysType type = sys_type_from_instance(self);

  return type == ttype;
}

FRWindow* fr_event_any_get_window(FREventAny *self) {
  sys_return_val_if_fail(self != NULL, NULL);


  return self->window;
}

/* object api */
static void fr_event_any_construct(FREvent *o, FRWindow *window, SysInt etype) {
  FR_EVENT_CLASS(fr_event_any_parent_class)->construct(o, window);

  FREventAny *self = FR_EVENT_ANY(o);

  self->window = window;
  self->etype = etype;
}

FREvent* fr_event_any_new(void) {
  return sys_object_new(FR_TYPE_EVENT_ANY, NULL);
}

FREvent *fr_event_any_new_I(FRWindow *window, SysInt etype) {
  FREvent *o = fr_event_any_new();

  fr_event_any_construct(o, window, etype);

  return o;
}

static void fr_event_any_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_event_any_parent_class)->dispose(o);
}

static void fr_event_any_class_init(FREventAnyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_event_any_dispose;
}

void fr_event_any_init(FREventAny *self) {
}

