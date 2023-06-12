#include <Framework/Event/Base/FREventScroll.h>

struct _FREventScrollPrivate {
  SysChar reserved;
};


SYS_DEFINE_TYPE_WITH_PRIVATE(FREventScroll, fr_event_scroll, FR_TYPE_EVENT);

/* object api */
static void fr_event_scroll_construct(SysObject *o, FRWindow *window) {
  SYS_OBJECT_CLASS(fr_event_scroll_parent_class)->construct(o, window);

  FREventScroll* self = FR_EVENT_SCROLL(o);
  FREventScrollPrivate* priv = self->priv;


}

FREventScroll* fr_event_scroll_new(void) {
  return sys_object_new(FR_TYPE_EVENT_SCROLL, NULL);
}

FREventScroll *fr_event_scroll_new_I(FRWindow *window) {
  FREventScroll *o = fr_event_scroll_new();

  fr_event_scroll_construct(SYS_OBJECT(o), window);

  return o;
}

static void fr_event_scroll_dispose(SysObject* o) {
  FREventScroll *self = FR_EVENT_SCROLL(o);
  FREventScrollPrivate* priv = self->priv;


  SYS_OBJECT_CLASS(fr_event_scroll_parent_class)->dispose(o);
}

static void fr_event_scroll_class_init(FREventScrollClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)fr_event_scroll_construct;
  ocls->dispose = fr_event_scroll_dispose;
}

void fr_event_scroll_init(FREventScroll *self) {
  self->priv = fr_event_scroll_get_private(self);
}

