#include <Framework/Media/FrPipeline.h>

SYS_DEFINE_TYPE(FrPipeline, fr_pipeline, SYS_TYPE_OBJECT);

/* object api */
static void fr_pipeline_construct(FrPipeline *self) {
}

FrPipeline* fr_pipeline_new(void) {
  return sys_object_new(FR_TYPE_PIPELINE, NULL);
}

FrPipeline *fr_pipeline_new_I(void) {
  FrPipeline *o = fr_pipeline_new();

  fr_pipeline_construct(o);

  return o;
}

static void fr_pipeline_dispose(SysObject* o) {


}

static void fr_pipeline_class_init(FrPipelineClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_pipeline_dispose;
}

void fr_pipeline_init(FrPipeline* self) {
}
