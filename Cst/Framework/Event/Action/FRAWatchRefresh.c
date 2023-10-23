#include <Framework/Event/Action/FRAWatchRefresh.h>
#include <Framework/Event/Action/FRAction.h>
#include <Framework/Event/Base/FREventRefresh.h>


SYS_DEFINE_TYPE(FRAWatchRefresh, fr_awatch_refresh, FR_TYPE_AWATCH);


static SysBool fr_awatch_refresh_check_i(FRAWatch *o, FREvent *e) {
  FRAWatchRefresh *self = FR_AWATCH_REFRESH(o);
  SysInt64 current;

  if(!fr_event_is(e, FR_TYPE_EVENT_REFRESH)) {
    return false;
  }

  if(self->is_refreshing) {
    return false;
  }

  current = sys_get_monotonic_time();
  if (self->last_clock == 0) {
    self->last_clock = current;
    return true;
  }

  SysBool r = (current - self->last_clock) > self->rate_time;
  self->last_clock = r ? current : self->last_clock;

  return r;
}

FRAWatch *fr_awatch_refresh_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_REFRESH, NULL);
}

SysObject *fr_awatch_refresh_clone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  SysObject *n = SYS_OBJECT_CLASS(fr_awatch_refresh_parent_class)->dclone(o);

  FRAWatchRefresh *nself = FR_AWATCH_REFRESH(n);
  FRAWatchRefresh *oself = FR_AWATCH_REFRESH(o);

  nself->last_clock = oself->last_clock;
  nself->rate_time = oself->rate_time;
  nself->is_refreshing = oself->is_refreshing;
  nself->app_data = oself->app_data;

  return n;
}

void fr_awatch_refresh_dispatch_i (FRAWatch *o, FREvent *e) {
  FR_AWATCH_CLASS(fr_awatch_refresh_parent_class)->dispatch(o, e);

  FRAWatchRefresh *self = FR_AWATCH_REFRESH(o);

  self->is_refreshing = false;
}

/* object api */
static void fr_awatch_refresh_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_awatch_refresh_parent_class)->dispose(o);
}

void fr_awatch_refresh_create_i(FRAWatch* o, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  FRAWatchRefresh *self = FR_AWATCH_REFRESH(o);

  FR_AWATCH_CLASS(fr_awatch_refresh_parent_class)->create(o, func_name, func, props);

  self->rate_time = (1 / 70.0) * 1e3;
}

static void fr_awatch_refresh_class_init(FRAWatchRefreshClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  ocls->dispose = fr_awatch_refresh_dispose;
  ocls->dclone = fr_awatch_refresh_clone_i;

  wcls->create = fr_awatch_refresh_create_i;
  wcls->check = fr_awatch_refresh_check_i;
  wcls->dispatch = fr_awatch_refresh_dispatch_i;
}

static void fr_awatch_refresh_init(FRAWatchRefresh *self) {
  fr_awatch_set_action(FR_AWATCH(self), FR_ACTION_STATIC);
}
