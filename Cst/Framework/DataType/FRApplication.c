#include <Framework/DataType/FRApplication.h>
#include <Framework/Event/FREvents.h>

struct _FRApplicationPrivate {
  SysPointer app_data;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRApplication, fr_application, FR_TYPE_SOURCE);

SysBool fr_application_check_i(FRSource *o) {

  if (fr_events_check()) {
    return true;
  }
  fr_wait_events();

  return true;
}

SysBool fr_application_prepare_i(FRSource *o) {
  return true;
}

void fr_application_finish_i(FRSource *o) {
}

SysBool fr_application_dispatch_i(FRSource *o) {
  FREvent* event = fr_events_get();

  if (event == NULL) {
    return false;
  }

  fr_events_dispatch(event);

  return true;
}

/* object api */
static void fr_application_construct(FRSource *o, SysPointer app_data) {
  FR_SOURCE_CLASS(fr_application_parent_class)->construct(o, (SysPointer)o);

  FRApplication *self = FR_APPLICATION(o);
  FRApplicationPrivate* priv = self->priv;

  priv->app_data = app_data;
}

FRSource* fr_application_new(void) {
  return sys_object_new(FR_TYPE_APPLICATION, NULL);
}

FRSource *fr_application_new_I(SysPointer app_data) {
  sys_return_val_if_fail(app_data != NULL, NULL);

  FRSource *o = fr_application_new();

  fr_application_construct(o, app_data);

  return o;
}

static void fr_application_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_application_parent_class)->dispose(o);
}

static void fr_application_class_init(FRApplicationClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRSourceClass *scls = FR_SOURCE_CLASS(cls);

  scls->prepare = fr_application_prepare_i;
  scls->check = fr_application_check_i;
  scls->dispatch = fr_application_dispatch_i;
  scls->finish = fr_application_finish_i;
  scls->construct = fr_application_construct;

  ocls->dispose = fr_application_dispose;
}

void fr_application_init(FRApplication *self) {
  self->priv = fr_application_get_private(self);
}
