#include <Framework/Event/Base/FREventRefresh.h>


struct _FREventRefreshPrivate {
  SysChar reserved;
};


SYS_DEFINE_TYPE_WITH_PRIVATE(FREventRefresh, fr_event_refresh, FR_TYPE_EVENT);


/* object api */
static void fr_event_refresh_construct(SysObject *o, FRWindow *window) {
  SYS_OBJECT_CLASS(fr_event_refresh_parent_class)->construct(o, window, FR_EVENT_TWINDOW_REFRESH);
}

FREvent* fr_event_refresh_new(void) {
  return sys_object_new(FR_TYPE_EVENT_REFRESH, NULL);
}

FREvent * fr_event_refresh_new_I(FRWindow *window) {
  FREvent *o = fr_event_refresh_new();

  fr_event_refresh_construct(SYS_OBJECT(o), window);

  return o;
}

static void fr_event_refresh_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(fr_event_refresh_parent_class)->dispose(o);
}

static void fr_event_refresh_class_init(FREventRefreshClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)fr_event_refresh_construct;
  ocls->dispose = fr_event_refresh_dispose;
}

void fr_event_refresh_init(FREventRefresh *self) {
  self->priv = fr_event_refresh_get_private(self);
}
