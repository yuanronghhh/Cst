#include <Framework/Event/Action/FRAWatchRefresh.h>
#include <Framework/Event/Action/FRAction.h>
#include <Framework/Event/Base/FREvent.h>


struct _FRAWatchRefreshPrivate {
  SysPointer app_data;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRAWatchRefresh, fr_awatch_refresh, FR_TYPE_AWATCH);

static SysBool fr_awatch_refresh_check_i(FRAWatch *o, FREvent *e) {
  FRAWatchRefresh *self = FR_AWATCH_REFRESH(o);
  FRAWatchRefreshPrivate *priv = self->priv;

  if(!fr_event_is(e, FR_TYPE_EVENT_REFRESH)) {
    return false;
  }

  if(priv->is_refreshing) {
    return false;
  }

  return true;
}

FRAWatch *fr_awatch_refresh_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_REFRESH, NULL);
}

FRAWatch *fr_awatch_refresh_clone_i(FRAWatch *old) {
  sys_return_val_if_fail(old != NULL, NULL);

  SysType type = sys_type_from_instance(old);
  FRAWatch *nwatch = sys_object_new(type, NULL);

  return nwatch;
}

/* object api */
static void fr_awatch_refresh_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_awatch_refresh_parent_class)->dispose(o);
}

void fr_awatch_refresh_create_i(FRAWatch* o, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  FR_AWATCH_CLASS(fr_awatch_refresh_parent_class)->create(o, func_name, func, props);
}

static void fr_awatch_refresh_class_init(FRAWatchRefreshClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  ocls->dispose = fr_awatch_refresh_dispose;

  wcls->dclone = fr_awatch_refresh_clone_i;
  wcls->create = fr_awatch_refresh_create_i;
  wcls->check = fr_awatch_refresh_check_i;
}

static void fr_awatch_refresh_init(FRAWatchRefresh *self) {
  self->priv = fr_awatch_refresh_get_private(self);

  fr_awatch_set_action(FR_AWATCH(self), FR_ACTION_STATIC);
}
