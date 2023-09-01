#include <Framework/DataType/FRMain.h>
#include <Framework/DataType/FRSource.h>

#define SOURCE_LOCK(m) fr_main_lock((m)->priv->main)
#define SOURCE_UNLOCK(m) fr_main_unlock((m)->priv->main)


struct _FRSourcePrivate {
  FRMain *main;
  SysPointer user_data;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRSource, fr_source, SYS_TYPE_OBJECT);

SysBool fr_source_check(FRSource *source) {
  sys_return_val_if_fail(source, false);

  FRSourceClass *cls = FR_SOURCE_GET_CLASS(source);
  SysBool rs = false;

  SOURCE_LOCK(source);
  if (cls->check) {
    rs = cls->check(source);
  }
  SOURCE_UNLOCK(source);

  return rs;
}

void fr_source_prepare(FRSource *source) {
  sys_return_if_fail(source != NULL);

  FRSourceClass *cls = FR_SOURCE_GET_CLASS(source);

  SOURCE_LOCK(source);
  if (cls->prepare) {
    cls->prepare(source);
  }
  SOURCE_UNLOCK(source);
}

void fr_source_finish(FRSource *source) {
  sys_return_if_fail(source != NULL);

  FRSourceClass *cls = FR_SOURCE_GET_CLASS(source);

  SOURCE_LOCK(source);
  if (cls->finish) {
    cls->finish(source);
  }
  SOURCE_UNLOCK(source);
}

void fr_source_dispatch(FRSource *source) {
  sys_return_if_fail(source != NULL);

  FRSourceClass *cls = FR_SOURCE_GET_CLASS(source);

  SOURCE_LOCK(source);
  if (cls->dispatch) {
    cls->dispatch(source);
  }
  SOURCE_UNLOCK(source);
}

void fr_source_set_main(FRSource *source, FRMain *main) {
  sys_return_if_fail(source != NULL);
  sys_return_if_fail(main != NULL);

  source->priv->main = main;
  sys_object_ref(main);
}

/* object api */
static void fr_source_construct(FRSource *self, SysPointer user_data) {
  FRSourcePrivate* priv = self->priv;

  priv->main = NULL;
  priv->user_data = user_data;
}

FRSource* fr_source_new(void) {
  return sys_object_new(FR_TYPE_SOURCE, NULL);
}

FRSource *fr_source_new_I(SysPointer user_data) {
  FRSource *o = fr_source_new();

  fr_source_construct(o, user_data);

  return o;
}

static void fr_source_dispose(SysObject* o) {
  FRSource *self = FR_SOURCE(o);
  FRSourcePrivate* priv = self->priv;

  sys_object_unref(priv->main);

  SYS_OBJECT_CLASS(fr_source_parent_class)->dispose(o);
}

static void fr_source_class_init(FRSourceClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_source_construct;
  ocls->dispose = fr_source_dispose;
}

void fr_source_init(FRSource *self) {
  self->priv = fr_source_get_private(self);
}
