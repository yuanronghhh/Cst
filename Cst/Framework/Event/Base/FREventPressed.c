#include <Framework/Event/Base/FREventPressed.h>

struct _FREventPressedPrivate {
  SysInt reserved;
};


SYS_DEFINE_TYPE_WITH_PRIVATE(FREventPressed, fr_event_pressed, FR_TYPE_EVENT);

/* object api */
static void fr_event_pressed_construct(SysObject *o, FRWindow *window) {
  SYS_OBJECT_CLASS(fr_event_pressed_parent_class)->construct(o, window);

  FREventPressed* self = FR_EVENT_PRESSED(o);
  FREventPressedPrivate* priv = self->priv;


}

FREventPressed* fr_event_pressed_new(void) {
  return sys_object_new(FR_TYPE_EVENT_PRESSED, NULL);
}

FREventPressed *fr_event_pressed_new_I(FRWindow *window) {
  FREventPressed *o = fr_event_pressed_new();

  fr_event_pressed_construct(SYS_OBJECT(o), window);

  return o;
}

static void fr_event_pressed_dispose(SysObject* o) {
  FREventPressed *self = FR_EVENT_PRESSED(o);
  FREventPressedPrivate* priv = self->priv;


  SYS_OBJECT_CLASS(fr_event_pressed_parent_class)->dispose(o);
}

static void fr_event_pressed_class_init(FREventPressedClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)fr_event_pressed_construct;
  ocls->dispose = fr_event_pressed_dispose;
}

void fr_event_pressed_init(FREventPressed *self) {
  self->priv = fr_event_pressed_get_private(self);
}

