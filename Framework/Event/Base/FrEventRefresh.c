#include <Framework/Event/Base/FrEventRefresh.h>

SYS_DEFINE_TYPE(FrEventRefresh, fr_event_refresh, FR_TYPE_EVENT);

/* object api */

static void fr_event_refresh_construct_i(FrEvent *o,
    FrWindow *window) {

  FR_EVENT_CLASS(fr_event_refresh_parent_class)->construct(o, window);
}

static void fr_event_refresh_construct(FrEvent *o,
    FrWindow *window,
    FrBound *bound) {
  FrEventRefresh *self = FR_EVENT_REFRESH(o);

  fr_event_refresh_construct_i(o, window);

  self->bound = bound;
}

FrEvent* fr_event_refresh_new(void) {
  return sys_object_new(FR_TYPE_EVENT_REFRESH, NULL);
}

FrEvent * fr_event_refresh_new_I(FrWindow *window) {
  FrEvent *o = fr_event_refresh_new();

  fr_event_refresh_construct(o, window, NULL);

  return o;
}

FrEvent *fr_event_refresh_new_bound(FrWindow *window, FrBound *bound) {
  FrEvent *o = fr_event_refresh_new();

  fr_event_refresh_construct(o, window, bound);

  return o;
}

static void fr_event_refresh_dispose(SysObject* o) {

}

static void fr_event_refresh_class_init(FrEventRefreshClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrEventClass *ecls = FR_EVENT_CLASS(cls);

  ecls->construct = fr_event_refresh_construct_i;
  ocls->dispose = fr_event_refresh_dispose;
}

void fr_event_refresh_init(FrEventRefresh *self) {
}

