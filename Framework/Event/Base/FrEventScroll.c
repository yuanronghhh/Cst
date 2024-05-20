#include <Framework/Event/Base/FrEventScroll.h>

struct _FrEventScrollPrivate {
  SysChar reserved;
};


SYS_DEFINE_TYPE_WITH_PRIVATE(FrEventScroll, fr_event_scroll, FR_TYPE_EVENT);

/* object api */
static void fr_event_scroll_construct(SysObject *o, FrWindow *window) {
  SYS_OBJECT_CLASS(fr_event_scroll_parent_class)->construct(o, window);

  FrEventScroll* self = FR_EVENT_SCROLL(o);
  FrEventScrollPrivate* priv = self->priv;


}

FrEventScroll* fr_event_scroll_new(void) {
  return sys_object_new(FR_TYPE_EVENT_SCROLL, NULL);
}

FrEventScroll *fr_event_scroll_new_I(FrWindow *window) {
  FrEventScroll *o = fr_event_scroll_new();

  fr_event_scroll_construct(o, window);

  return o;
}

static void fr_event_scroll_dispose(SysObject* o) {
  FrEventScroll *self = FR_EVENT_SCROLL(o);
  FrEventScrollPrivate* priv = self->priv;


  SYS_OBJECT_CLASS(fr_event_scroll_parent_class)->dispose(o);
}

static void fr_event_scroll_class_init(FrEventScrollClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_event_scroll_construct;
  ocls->dispose = fr_event_scroll_dispose;
}

void fr_event_scroll_init(FrEventScroll *self) {
  self->priv = fr_event_scroll_get_private(self);
}

