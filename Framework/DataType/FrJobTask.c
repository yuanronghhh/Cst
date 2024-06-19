#include <Framework/DataType/FrJobTask.h>


SYS_DEFINE_TYPE(FrJobTask, fr_job_task, SYS_TYPE_OBJECT);

static SysBool fr_job_task_destroy_i(SysObject* o) {
  FrJobTask *self = FR_JOB_TASK(o);

  self->result = NULL;
  self->user_data = NULL;
  self->handler = NULL;
  self->callback = NULL;

  return true;
}

void fr_job_task_run(FrJobTask *self) {
  sys_return_if_fail(self != NULL);

  if(self->handler) {

    self->result = self->handler(self, self->user_data);
  }

  if(self->callback) {

    self->callback(self, self->user_data);
  }
  sys_mutex_lock(&self->mutex);

  if(self->done) {

    goto done;
  }
  self->done = true;

  if(self->is_sync) {

    sys_cond_signal(&self->cond);
  }

done:
  sys_mutex_unlock(&self->mutex);
}

void fr_job_task_run_sync(FrJobTask *self) {
  fr_job_task_run(self);
  fr_job_task_wait(self);
}

void fr_job_task_run_async(FrJobTask *self) {
  fr_job_task_run(self);
}

void fr_job_task_setup(void) {
}

void fr_job_task_teardown(void) {
}

SysPointer fr_job_task_result(FrJobTask *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->result;
}

void fr_job_task_construct(FrJobTask *self, FrJobTaskContext *info) {
  sys_return_if_fail(self != NULL);

  FrJobTaskClass* cls = FR_JOB_TASK_GET_CLASS(self);
  sys_return_if_fail(cls->construct != NULL);

  return cls->construct(self, info);
}

void fr_job_task_wait(FrJobTask *self) {
  sys_mutex_lock(&self->mutex);

  while(!self->done) {

    sys_cond_wait(&self->cond, &self->mutex);
  }

  sys_mutex_unlock(&self->mutex);
}

/* object api */
static void fr_job_task_construct_i(FrJobTask *self, FrJobTaskContext *info) {
  self->user_data = info->user_data;
  self->handler = info->handler;
  self->callback = info->callback;
  self->done = false;
}

FrJobTask* fr_job_task_new(void) {
  return sys_object_new(FR_TYPE_JOB_TASK, NULL);
}

FrJobTask *fr_job_task_new_handler(FrJobTaskFunc handler, 
    SysPointer user_data) {

  FrJobTaskContext info = {0};
  info.handler = handler;
  info.user_data = user_data;

  return fr_job_task_new_I(&info);
}

FrJobTask *fr_job_task_new_I(FrJobTaskContext *info) {
  FrJobTask *o = fr_job_task_new();

  fr_job_task_construct_i(o, info);

  return o;
}

static void fr_job_task_dispose(SysObject* o) {
  FrJobTask *self = FR_JOB_TASK(o);

  sys_cond_clear(&self->cond);
  sys_mutex_clear(&self->mutex);

  SYS_OBJECT_CLASS(fr_job_task_parent_class)->dispose(o);
}

static void fr_job_task_class_init(FrJobTaskClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_job_task_dispose;
  ocls->destroy = fr_job_task_destroy_i;
  cls->construct = fr_job_task_construct_i;
}

void fr_job_task_init(FrJobTask* self) {
  self->done = false;

  sys_cond_init(&self->cond);
  sys_mutex_init(&self->mutex);
}
