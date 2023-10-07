#include <Framework/Event/Base/FREventMouseKey.h>

SYS_DEFINE_TYPE(FREventMouseKey, fr_event_mousekey, FR_TYPE_EVENT);


SysInt fr_event_mousekey_key(FREventMouseKey *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->button;
}

SysInt fr_event_mousekey_action(FREventMouseKey *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->action;
}

/* object api */
static void fr_event_mousekey_construct(FREvent *o, FRWindow *window, SysInt button, SysInt action, SysInt mods) {
  FR_EVENT_CLASS(fr_event_mousekey_parent_class)->construct(o, window);

  FREventMouseKey* self = FR_EVENT_MOUSEKEY(o);

  self->button = button;
  self->action = action;
  self->mods = mods;
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
}

