#include <Framework/Event/Action/FrAWatchRefresh.h>
#include <Framework/Event/Action/FrAction.h>
#include <Framework/Event/Base/FrEventRefresh.h>


SYS_DEFINE_TYPE(FrAWatchRefresh, fr_awatch_refresh, FR_TYPE_AWATCH);


static SysBool fr_awatch_refresh_check_i(FrAWatch *o, FrEvent *e) {
  SysInt64 current;
  SysInt64 diff;
  FrAWatchRefresh *self = FR_AWATCH_REFRESH(o);

  current = sys_get_monotonic_time();
  diff = (current - self->last_clock);

  if(diff < self->rate_time) {
    return false;
  }

  if (diff < self->cost_time) {
    return false;
  }

  self->last_clock = current;

  return true;
}

FrAWatch *fr_awatch_refresh_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_REFRESH, NULL);
}

SysObject *fr_awatch_refresh_clone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  SysObject *n = SYS_OBJECT_CLASS(fr_awatch_refresh_parent_class)->dclone(o);

  FrAWatchRefresh *nself = FR_AWATCH_REFRESH(n);
  FrAWatchRefresh *oself = FR_AWATCH_REFRESH(o);

  nself->last_clock = oself->last_clock;
  nself->rate_time = oself->rate_time;
  nself->is_refreshing = oself->is_refreshing;
  nself->app_data = oself->app_data;

  return n;
}

void fr_awatch_refresh_dispatch_i (FrAWatch *o, FrEvent *e) {
  FrAWatchRefresh *self = FR_AWATCH_REFRESH(o);
  SysInt64 start;
  SysInt64 end;

  start = sys_get_monotonic_time();
  FR_AWATCH_CLASS(fr_awatch_refresh_parent_class)->dispatch(o, e);
  end = sys_get_monotonic_time();

  self->cost_time = end - start;
}

/* object api */
static void fr_awatch_refresh_dispose(SysObject* o) {
}

void fr_awatch_refresh_construct_i(FrAWatch* o, FrAWatchBuilder *builder) {
  FrAWatchRefresh *self = FR_AWATCH_REFRESH(o);

  FR_AWATCH_CLASS(fr_awatch_refresh_parent_class)->construct(o, builder);

  self->rate_time = (1 / 70.0) * 1e6;
}

static void fr_awatch_refresh_class_init(FrAWatchRefreshClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  ocls->dispose = fr_awatch_refresh_dispose;
  ocls->dclone = fr_awatch_refresh_clone_i;

  wcls->construct = fr_awatch_refresh_construct_i;
  wcls->check = fr_awatch_refresh_check_i;
  wcls->dispatch = fr_awatch_refresh_dispatch_i;
}

static void fr_awatch_refresh_init(FrAWatchRefresh *self) {

  fr_awatch_set_action(FR_AWATCH(self), FR_ACTION_STATIC);
}
