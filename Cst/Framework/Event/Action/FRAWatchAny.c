#include <Framework/Event/Action/FRAWatchAny.h>

#include <Framework/Event/Action/FRAction.h>
#include <Framework/Event/Base/FREvent.h>


SYS_DEFINE_TYPE(FRAWatchAny, fr_awatch_any, FR_TYPE_AWATCH);

static SysBool fr_awatch_any_check_i(FRAWatch *o, FREvent *e) {
  FRAWatchAny *self = FR_AWATCH_ANY(o);

  return fr_event_is(e, self->etype);
}

FRAWatch *fr_awatch_any_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_ANY, NULL);
}

SysObject *fr_awatch_any_clone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  SysObject *no = SYS_OBJECT_CLASS(fr_awatch_any_parent_class)->dclone(o);

  FRAWatchAny *oself = FR_AWATCH_ANY(o);
  FRAWatchAny *nself = FR_AWATCH_ANY(no);

  nself->etype = oself->etype;

  return no;
}

/* object api */
static void fr_awatch_any_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_awatch_any_parent_class)->dispose(o);
}


void fr_awatch_any_construct_i(FRAWatch* o, FRAWatchBuilder *builder) {
  FR_AWATCH_CLASS(fr_awatch_any_parent_class)->construct(o, builder);

  FRAWatchAny* self = FR_AWATCH_ANY(o);

  fr_awatch_builder_build_awatch_any(builder, self);
}

FRAWatch *fr_awatch_any_new_I(SysType etype, const SysChar *func_name, FREventFunc func) {
  FRAWatch *o = fr_awatch_any_new();

  FRAWatchBuilder *builder = fr_awatch_builder_new_I(func_name, func);
  fr_awatch_builder_set_etype(builder, etype);
  fr_awatch_any_construct_i(o, builder);
  sys_object_unref(builder);

  return o;
}

void fr_awatch_any_set_etype(FRAWatchAny *self, SysType etype) {
  sys_return_if_fail(self != NULL);

  self->etype = etype;
}

SysType fr_awatch_any_get_etype(FRAWatchAny *self) {
  sys_return_val_if_fail(self != NULL, 0);

  return self->etype;
}

static void fr_awatch_any_class_init(FRAWatchAnyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  ocls->dispose = fr_awatch_any_dispose;
  ocls->dclone = fr_awatch_any_clone_i;

  wcls->construct = fr_awatch_any_construct_i;
  wcls->check = fr_awatch_any_check_i;
}

static void fr_awatch_any_init(FRAWatchAny *self) {

  fr_awatch_set_action(FR_AWATCH(self), FR_ACTION_STATIC);
}
