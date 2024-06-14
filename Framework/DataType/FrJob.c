#include <Framework/DataType/FrJob.h>

SYS_DEFINE_TYPE(FrJob, fr_job, SYS_TYPE_OBJECT);

/* object api */
static void fr_job_construct_i(FrJob *self) {

}

FrJob* fr_job_new(void) {
  return sys_object_new(FR_TYPE_JOB, NULL);
}

FrJob *fr_job_new_I(void) {
  FrJob *o = fr_job_new();

  fr_job_construct_i(o);

  return o;
}

static void fr_job_dispose(SysObject* o) {
  FrJob *self = FR_JOB(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(fr_job_parent_class)->dispose(o);
}

static void fr_job_class_init(FrJobClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_job_dispose;
}

void fr_job_init(FrJob* self) {
}
