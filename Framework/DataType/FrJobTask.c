#include <Framework/DataType/FrJobTask.h>

static SysCond pool_cond;
static SysMutex pool_mutex;

SYS_DEFINE_TYPE(FrJobTask, fr_job_task, SYS_TYPE_OBJECT);

static SysBool fr_job_task_destroy_i(SysObject* o) {
  FrJobTask *self = FR_JOB_TASK(o);

  self->result = NULL;
  self->user_data = NULL;
  self->handler = NULL;
  self->callback = NULL;

  return true;
}

void fr_job_task_wait(FrJobTask* self, SysMutex *mutex) {
  sys_mutex_lock(mutex);

  while(!self->done) {

    sys_cond_wait(&pool_cond, mutex);
  }

  sys_mutex_unlock(mutex);
}

void fr_job_task_run(FrJobTask *self) {
  sys_return_if_fail(self != NULL);
  sys_mutex_lock(&pool_mutex);

  self->done = false;
  if(self->handler) {

    self->result = self->handler(self, self->user_data);
  }

  if(self->callback) {

    self->callback(self, self->user_data);
  }
  self->done = true;
  sys_cond_signal(&pool_cond);
  sys_mutex_unlock(&pool_mutex);
}

void fr_job_task_setup(void) {
  sys_cond_init(&pool_cond);
  sys_mutex_init(&pool_mutex);
}

void fr_job_task_teardown(void) {
  sys_cond_clear(&pool_cond);
  sys_mutex_clear(&pool_mutex);
}

SysPointer fr_job_task_result(FrJobTask *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->result;
}

/* object api */
static void fr_job_task_construct_i(FrJobTask *self) {

}

FrJobTask* fr_job_task_new(void) {
  return sys_object_new(FR_TYPE_JOB_TASK, NULL);
}

FrJobTask *fr_job_task_new_I(void) {
  FrJobTask *o = fr_job_task_new();

  fr_job_task_construct_i(o);

  return o;
}

static void fr_job_task_dispose(SysObject* o) {
  FrJobTask *self = FR_JOB_TASK(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(fr_job_task_parent_class)->dispose(o);
}

static void fr_job_task_class_init(FrJobTaskClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_job_task_dispose;
  ocls->destroy = fr_job_task_destroy_i;
}

void fr_job_task_init(FrJobTask* self) {
}
