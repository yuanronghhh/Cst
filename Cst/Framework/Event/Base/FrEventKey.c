#include <Framework/Event/Base/FrEventKey.h>


SYS_DEFINE_TYPE(FrEventKey, fr_event_key, FR_TYPE_EVENT);


SysInt fr_event_key_scancode(FrEventKey *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->scancode;
}

SysInt fr_event_key_key(FrEventKey *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->key;
}

SysInt fr_event_key_action(FrEventKey *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->action;
}

/* object api */
static void fr_event_key_construct(FrEvent *o, FrWindow *window, SysInt key, SysInt scancode, SysInt action, SysInt mods) {
  FR_EVENT_CLASS(fr_event_key_parent_class)->construct(o, window);

  FrEventKey *self = FR_EVENT_KEY(o);

  self->key = key;
  self->scancode = scancode;
  self->action = action;
  self->mods = mods;
}

FrEvent* fr_event_key_new(void) {
  return sys_object_new(FR_TYPE_EVENT_KEY, NULL);
}

FrEvent *fr_event_key_new_I(FrWindow *window, SysInt key, SysInt scancode, SysInt action, SysInt mods) {
  FrEvent *o = fr_event_key_new();

  fr_event_key_construct(o, window, key, scancode, action, mods);

  return o;
}

static void fr_event_key_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_event_key_parent_class)->dispose(o);
}

static void fr_event_key_class_init(FrEventKeyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_event_key_dispose;
}

void fr_event_key_init(FrEventKey *self) {
}

