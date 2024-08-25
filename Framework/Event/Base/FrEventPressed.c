#include <Framework/Event/Base/FrEventPressed.h>

struct _FrEventPressedPrivate {
  SysChar reserved;
};


SYS_DEFINE_TYPE_WITH_PRIVATE(FrEventPressed, fr_event_pressed, FR_TYPE_EVENT);

/* object api */
static void fr_event_pressed_construct(SysObject *o, FrWindow *window) {
  SYS_OBJECT_CLASS(fr_event_pressed_parent_class)->construct(o, window);

  FrEventPressed* self = FR_EVENT_PRESSED(o);
  FrEventPressedPrivate* priv = self->priv;


}

FrEventPressed* fr_event_pressed_new(void) {
  return sys_object_new(FR_TYPE_EVENT_PRESSED, NULL);
}

FrEventPressed *fr_event_pressed_new_I(FrWindow *window) {
  FrEventPressed *o = fr_event_pressed_new();

  fr_event_pressed_construct(o, window);

  return o;
}

static void fr_event_pressed_dispose(SysObject* o) {
  FrEventPressed *self = FR_EVENT_PRESSED(o);
  FrEventPressedPrivate* priv = self->priv;



}

static void fr_event_pressed_class_init(FrEventPressedClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_event_pressed_construct;
  ocls->dispose = fr_event_pressed_dispose;
}

void fr_event_pressed_init(FrEventPressed *self) {
  self->priv = fr_event_pressed_get_private(self);
}

