#include <Framework/Event/Action/FRAWatchBuilder.h>


SYS_DEFINE_TYPE(FRAWatchBuilder, fr_awatch_builder, SYS_TYPE_OBJECT);


/* object api */

static void fr_awatch_builder_construct(FRAWatchBuilder* self, const SysChar *func_name, FREventFunc func) {
  self->user_data = NULL;
  self->func = func;
  self->func_name = sys_strdup(func_name);
}

static void fr_awatch_builder_dispose(SysObject* o) {
  FRAWatchBuilder *self = FR_AWATCH_BUILDER(o);

  SYS_OBJECT_CLASS(fr_awatch_builder_parent_class)->dispose(o);
}

FRAWatchBuilder *fr_awatch_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_BUILDER, NULL);
}

FRAWatchBuilder *fr_awatch_builder_new_I(const SysChar *func_name, FREventFunc func) {
  FRAWatchBuilder *o = fr_awatch_builder_new();

  fr_awatch_builder_construct(o, func_name, func);

  return o;
}

static void fr_awatch_builder_class_init(FRAWatchBuilderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_awatch_builder_dispose;
  ocls->dclone = fr_awatch_builder_dclone_i;
}

static void fr_awatch_builder_init(FRAWatchBuilder *self) {
}
