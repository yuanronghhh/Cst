#include <Framework/DataType/FrApplication.h>
#include <Framework/Device/FrWindow.h>
#include <Framework/Event/FrEvents.h>


SYS_DEFINE_TYPE(FrApplication, fr_application, FR_TYPE_SOURCE);


SysBool fr_application_check_i(FrSource *o) {

  if (fr_events_check()) {
    return true;
  }
  fr_wait_events();

  return true;
}

SysBool fr_application_prepare_i(FrSource *o) {
  return true;
}

void fr_application_finish_i(FrSource *o) {
}

SysBool fr_application_dispatch_i(FrSource *o) {
  FrEvent* event = fr_events_get();

  if (event == NULL) {
    return false;
  }

  fr_events_dispatch(event);

  return true;
}

/* object api */
static void fr_application_construct(FrSource *o, SysPointer app_data) {
  FR_SOURCE_CLASS(fr_application_parent_class)->construct(o, (SysPointer)o);

  FrApplication *self = FR_APPLICATION(o);

  self->app_data = app_data;
}

FrSource* fr_application_new(void) {
  return sys_object_new(FR_TYPE_APPLICATION, NULL);
}

FrSource *fr_application_new_I(SysPointer app_data) {
  sys_return_val_if_fail(app_data != NULL, NULL);

  FrSource *o = fr_application_new();

  fr_application_construct(o, app_data);

  return o;
}

static void fr_application_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_application_parent_class)->dispose(o);
}

static void fr_application_class_init(FrApplicationClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrSourceClass *scls = FR_SOURCE_CLASS(cls);

  scls->prepare = fr_application_prepare_i;
  scls->check = fr_application_check_i;
  scls->dispatch = fr_application_dispatch_i;
  scls->finish = fr_application_finish_i;
  scls->construct = fr_application_construct;

  ocls->dispose = fr_application_dispose;
}

void fr_application_init(FrApplication *self) {
}
