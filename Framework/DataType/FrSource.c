#include <Framework/DataType/FrMain.h>
#include <Framework/DataType/FrSource.h>

#define SOURCE_LOCK(m) fr_main_lock((m)->main)
#define SOURCE_UNLOCK(m) fr_main_unlock((m)->main)


SYS_DEFINE_TYPE(FrSource, fr_source, SYS_TYPE_OBJECT);

SysBool fr_source_check(FrSource *source) {
  sys_return_val_if_fail(source, false);

  FrSourceClass *cls = FR_SOURCE_GET_CLASS(source);
  SysBool rs = false;

  SOURCE_LOCK(source);
  if (cls->check) {
    rs = cls->check(source);
  }
  SOURCE_UNLOCK(source);

  return rs;
}

void fr_source_prepare(FrSource *source) {
  sys_return_if_fail(source != NULL);

  FrSourceClass *cls = FR_SOURCE_GET_CLASS(source);

  SOURCE_LOCK(source);
  if (cls->prepare) {
    cls->prepare(source);
  }
  SOURCE_UNLOCK(source);
}

void fr_source_finish(FrSource *source) {
  sys_return_if_fail(source != NULL);

  FrSourceClass *cls = FR_SOURCE_GET_CLASS(source);

  SOURCE_LOCK(source);
  if (cls->finish) {
    cls->finish(source);
  }
  SOURCE_UNLOCK(source);
}

void fr_source_dispatch(FrSource *source) {
  sys_return_if_fail(source != NULL);

  FrSourceClass *cls = FR_SOURCE_GET_CLASS(source);

  SOURCE_LOCK(source);
  if (cls->dispatch) {
    cls->dispatch(source);
  }
  SOURCE_UNLOCK(source);
}

void fr_source_set_main(FrSource *source, FrMain *main) {
  sys_return_if_fail(source != NULL);
  sys_return_if_fail(main != NULL);

  source->main = sys_object_ref(main);
}

/* object api */
static void fr_source_construct(FrSource *self, SysPointer user_data) {
  self->main = NULL;
  self->user_data = user_data;
}

FrSource* fr_source_new(void) {
  return sys_object_new(FR_TYPE_SOURCE, NULL);
}

FrSource *fr_source_new_I(SysPointer user_data) {
  FrSource *o = fr_source_new();

  fr_source_construct(o, user_data);

  return o;
}

static void fr_source_dispose(SysObject* o) {
  FrSource *self = FR_SOURCE(o);
  sys_object_unref(self->main);


}

static void fr_source_class_init(FrSourceClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_source_construct;
  ocls->dispose = fr_source_dispose;
}

void fr_source_init(FrSource *self) {
}
