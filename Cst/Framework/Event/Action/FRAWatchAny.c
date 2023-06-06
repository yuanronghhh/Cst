#include <Framework/Event/Action/FRAWatchAny.h>
#include <Framework/Event/Action/FRAction.h>
#include <Framework/Event/Base/FREvent.h>


struct _FRAWatchAnyPrivate {
  SysType etype;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRAWatchAny, fr_awatch_any, FR_TYPE_AWATCH);

static SysBool fr_awatch_any_check_i(FRAWatch *o, FREvent *e) {
  FRAWatchAny *self = FR_AWATCH_ANY(o);
  FRAWatchAnyPrivate *priv = self->priv;

  return fr_event_is(e, priv->etype);
}

FRAWatch *fr_awatch_any_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_ANY, NULL);
}

FRAWatch *fr_awatch_any_clone_i(FRAWatch *old) {
  sys_return_val_if_fail(old != NULL, NULL);

  SysType type = sys_type_from_instance(old);
  FRAWatch *nwatch = sys_object_new(type, NULL);

  FRAWatchAnyPrivate *opriv = FR_AWATCH_ANY(old)->priv;
  FRAWatchAnyPrivate *npriv = FR_AWATCH_ANY(nwatch)->priv;

  npriv->etype = opriv->etype;

  return nwatch;
}

/* object api */
static void fr_awatch_any_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_awatch_any_parent_class)->dispose(o);
}

void fr_awatch_any_create_i(FRAWatch* o, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  FR_AWATCH_CLASS(fr_awatch_any_parent_class)->create(o, func_name, func, props);

  FRAWatchAny* self = FR_AWATCH_ANY(o);
  FRAWatchAnyPrivate* priv = self->priv;

  priv->etype = props->etype;
}

static void fr_awatch_any_class_init(FRAWatchAnyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  ocls->dispose = fr_awatch_any_dispose;

  wcls->dclone = fr_awatch_any_clone_i;
  wcls->create = fr_awatch_any_create_i;
  wcls->check = fr_awatch_any_check_i;
}

static void fr_awatch_any_init(FRAWatchAny *self) {
  self->priv = fr_awatch_any_get_private(self);

  fr_awatch_set_action(FR_AWATCH(self), FR_ACTION_STATIC);
}
