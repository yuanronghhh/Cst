#include <Framework/DataType/FrJob.h>

SYS_DEFINE_TYPE(FrJob, fr_job, SYS_TYPE_OBJECT);

#define JOB_LOCK sys_mutex_lock(&self->mutex)
#define JOB_UNLOCK sys_mutex_unlock(&self->mutex)

void fr_job_lock(FrJob *self) {

  JOB_LOCK;
}

void fr_job_unlock(FrJob *self) {

  JOB_UNLOCK;
}

void fr_job_run_task_wait(FrJob *self, FrJobTask *task) {

  fr_job_task_wait(task, &self->mutex);
}

void fr_job_run_task(FrJob *self, FrJobTask *task) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(task != NULL);

  JOB_LOCK;

  sys_queue_push_tail(&self->task_queue, task);
  self->task.handler = task->handler;
  self->state = FR_JOB_STATE_RUNNING;
  sys_cond_signal(&self->cond);

  JOB_UNLOCK;
}

static SysPointer job_thread(SysPointer user_data) {
  FrJob *self = user_data;

  while (self->state == FR_JOB_STATE_RUNNING) {
    if(sys_queue_get_length(&self->task_queue) > 0) {

      fr_job_task_run(&self->task);
    }

    JOB_LOCK;
    self->func(self->user_data);
    JOB_UNLOCK;
  }

  return NULL;
}

static SysPointer stop_it(FrJobTask *task, SysPointer user_data) {
  FrJob *self = user_data;
  self->state = FR_JOB_STATE_STOP;
  return NULL;
}

static SysPointer job_callback (FrJobTask* task, SysPointer user_data) {
  FrJob *self = user_data;

  if(!self->callback) {
    return NULL;
  }

  return self->callback(self, self->user_data);
}

void fr_job_set_callback(FrJob* self, FrJobFunc callback) {
  sys_return_if_fail(callback != NULL);

  self->callback = callback;
}

void fr_job_stop(FrJob* self) {
  sys_return_if_fail(self != NULL);
  if (self->state == FR_JOB_STATE_STOP) { return; }

  self->task.handler = stop_it;
  self->user_data = NULL;

  fr_job_run_task_wait(self, &self->task);
}

void fr_job_start(FrJob *self, const SysChar *name, SysThreadFunc func) {
  SysThread* thread = sys_thread_new(name, job_thread, self);
  if (self->thread == NULL) {
    sys_error_N("job start failed: \"%s\"", name);
  }
  self->thread = thread;
}

void fr_job_join(FrJob *self) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->thread != NULL);

  sys_thread_join(self->thread);
}

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
  sys_return_if_fail(self->state == FR_JOB_STATE_STOP);

  sys_cond_clear(&self->cond);
  sys_mutex_clear(&self->mutex);
  sys_object_destroy(&self->task);

  self->user_data = NULL;
  self->callback = NULL;
  self->thread = NULL;

  SYS_OBJECT_CLASS(fr_job_parent_class)->dispose(o);
}

static void fr_job_class_init(FrJobClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_job_dispose;
}

void fr_job_init(FrJob* self) {

  fr_job_task_create(&self->task);

  self->task.user_data = self;
  self->task.callback = job_callback;
}
