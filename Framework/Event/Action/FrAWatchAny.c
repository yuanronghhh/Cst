#include <Framework/Event/Action/FrAWatchAny.h>

#include <Framework/Event/Action/FrAction.h>
#include <Framework/Event/Base/FrEventAny.h>
#include <Framework/Event/Base/FrEvent.h>


SYS_DEFINE_TYPE(FrAWatchAny, fr_awatch_any, FR_TYPE_AWATCH);

static SysBool fr_awatch_any_check_i(FrAWatch *o, FrEvent *e) {
  if (!fr_event_is(e, FR_TYPE_EVENT_ANY)) {
    return false;
  }

  FrAWatchAny *self = FR_AWATCH_ANY(o);
  FrEventAny *aevent = FR_EVENT_ANY(e);

  return aevent->event_enum == self->event_enum;
}

FrAWatch *fr_awatch_any_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_ANY, NULL);
}

SysObject *fr_awatch_any_clone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  SysObject *no = SYS_OBJECT_CLASS(fr_awatch_any_parent_class)->dclone(o);

  FrAWatchAny *oself = FR_AWATCH_ANY(o);
  FrAWatchAny *nself = FR_AWATCH_ANY(no);

  nself->event_enum = oself->event_enum;

  return no;
}

/* object api */
static void fr_awatch_any_dispose(SysObject* o) {


}

void fr_awatch_any_construct_i(FrAWatch* o, FrAWatchBuilder *builder) {
  FR_AWATCH_CLASS(fr_awatch_any_parent_class)->construct(o, builder);

  FrAWatchAny* self = FR_AWATCH_ANY(o);

  fr_awatch_builder_build_awatch_any(builder, self);
}

FrAWatch *fr_awatch_any_new_I(SysInt event_enum, const SysChar *func_name, FrEventFunc func) {
  FrAWatch *o = fr_awatch_any_new();

  FrAWatchBuilder *builder = fr_awatch_builder_new_I(func_name, func);
  fr_awatch_builder_set_event_enum(builder, event_enum);
  fr_awatch_any_construct_i(o, builder);
  sys_object_unref(builder);

  return o;
}

void fr_awatch_any_set_event_enum(FrAWatchAny *self, SysInt event_enum) {
  sys_return_if_fail(self != NULL);

  self->event_enum = event_enum;
}

SysInt fr_awatch_any_get_event_enum(FrAWatchAny *self) {
  sys_return_val_if_fail(self != NULL, 0);

  return self->event_enum;
}

static void fr_awatch_any_class_init(FrAWatchAnyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  ocls->dispose = fr_awatch_any_dispose;
  ocls->dclone = fr_awatch_any_clone_i;

  wcls->construct = fr_awatch_any_construct_i;
  wcls->check = fr_awatch_any_check_i;
}

static void fr_awatch_any_init(FrAWatchAny *self) {

  fr_awatch_set_action(FR_AWATCH(self), FR_ACTION_STATIC);
}
