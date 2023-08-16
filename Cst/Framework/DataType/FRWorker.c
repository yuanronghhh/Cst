#include <Framework/DataType/FRWorker.h>
#include <Framework/Event/FREvents.h>

struct _FRWorkerPrivate {
  SysPointer app_data;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRWorker, fr_worker, FR_TYPE_SOURCE);

SysBool fr_worker_check_i(FRSource *o) {

  if (fr_events_check()) {
    return true;
  }

  return true;
}

SysBool fr_worker_prepare_i(FRSource *o) {
  return true;
}

void fr_worker_finish_i(FRSource *o) {
}

SysBool fr_worker_dispatch_i(FRSource *o) {
  FREvent* event = fr_events_get();

  if (event == NULL) {
    return false;
  }

  fr_events_dispatch(event);

  return true;
}

/* object api */
void fr_worker_construct(FRSource *o, SysPointer app_data) {
  FR_SOURCE_CLASS(fr_worker_parent_class)->construct(o, (SysPointer)o);

  FRWorker* self = FR_WORKER(o);
  FRWorkerPrivate* priv = self->priv;

  priv->app_data = app_data;
}

FRSource* fr_worker_new(void) {
  return sys_object_new(FR_TYPE_WORKER, NULL);
}

FRSource *fr_worker_new_I(SysPointer app_data) {
  sys_return_val_if_fail(app_data != NULL, NULL);

  FRSource *o = fr_worker_new();

  fr_worker_construct(o, app_data);

  return o;
}

static void fr_worker_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_worker_parent_class)->dispose(o);
}

static void fr_worker_class_init(FRWorkerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRSourceClass *scls = FR_SOURCE_CLASS(cls);

  scls->prepare = fr_worker_prepare_i;
  scls->check = fr_worker_check_i;
  scls->dispatch = fr_worker_dispatch_i;
  scls->finish = fr_worker_finish_i;
  scls->construct = fr_worker_construct;

  ocls->dispose = fr_worker_dispose;
}

void fr_worker_init(FRWorker *self) {
  self->priv = fr_worker_get_private(self);
}
