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

void fr_job_wakeup_unlock(FrJob* self) {
  sys_return_if_fail(self != NULL);

  sys_cond_signal(&self->cond);
}

void fr_job_wakeup(FrJob *self) {
  sys_return_if_fail(self != NULL);

  JOB_LOCK;
  sys_cond_signal(&self->cond);
  JOB_UNLOCK;
}

void fr_job_send_task_unlock(FrJob *self, FrJobTask *task) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(task != NULL);

  self->task = task;
  sys_cond_signal(&self->cond);
}

void fr_job_send_task(FrJob *self, FrJobTask *task) {
  JOB_LOCK;

  fr_job_send_task_unlock(self, task);

  JOB_UNLOCK;
}

void fr_job_send_task_wait(FrJob *self, FrJobTask *task) {
  fr_job_send_task(self, task);

  fr_job_task_wait(task);
}

static SysPointer job_thread(SysPointer user_data) {
  FrJob *self = user_data;

  while (self->state == FR_JOB_STATE_RUNNING) {
    JOB_LOCK;

    if(sys_queue_get_length(&self->task_queue) > 0) {

      return;
    }

    fr_job_task_run(self->task);
    JOB_UNLOCK;
  }
  sys_debug_N("exit %s", self->name);

  return NULL;
}

static SysPointer stop_it(FrJobTask *task, SysPointer user_data) {
  FrJob *self = user_data;
  self->state = FR_JOB_STATE_STOP;
  return NULL;
}

void fr_job_stop(FrJob* self) {
  sys_return_if_fail(self != NULL);
  if (self->state == FR_JOB_STATE_STOP) { return; }
  FrJobTask *task;

  task = fr_job_task_new_callback(stop_it, self);
  fr_job_send_task_wait(self, task);
  sys_object_unref(task);
}

void fr_job_start(FrJob *self) {
  SysThread* thread = sys_thread_new(self->name, job_thread, self);
  if (thread == NULL) {
    sys_error_N("job start failed: \"%s\"", self->name);
  }
  self->thread = thread;
}

void fr_job_join(FrJob *self) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->thread != NULL);

  sys_thread_join(self->thread);
  sys_object_unref(self);
}

void fr_job_construct(FrJob *self, FrJobContext *info) {
  sys_return_if_fail(self != NULL);

  FrJobClass* cls = FR_JOB_GET_CLASS(self);
  sys_return_if_fail(cls->construct != NULL);

  return cls->construct(self, info);
}

/* object api */
static void fr_job_construct_i(FrJob *self, FrJobContext *info) {
  self->name = sys_strdup(info->name);
  self->user_data = info->user_data;
  self->callback = info->callback;
}

FrJob* fr_job_new(void) {
  return sys_object_new(FR_TYPE_JOB, NULL);
}

FrJob *fr_job_new_I(FrJobContext *info) {
  FrJob *o = fr_job_new();

  fr_job_construct_i(o, info);

  return o;
}

static void fr_job_dispose(SysObject* o) {
  FrJob *self = FR_JOB(o);
  sys_return_if_fail(self->state == FR_JOB_STATE_STOP);

  sys_cond_clear(&self->cond);
  sys_mutex_clear(&self->mutex);

  self->user_data = NULL;
  self->thread = NULL;
  sys_clear_pointer(&self->name, sys_free);

  SYS_OBJECT_CLASS(fr_job_parent_class)->dispose(o);
}

static void fr_job_class_init(FrJobClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_job_dispose;
  cls->construct = fr_job_construct_i;
}

void fr_job_init(FrJob* self) {
  self->state = FR_JOB_STATE_RUNNING;

  sys_cond_init(&self->cond);
  sys_mutex_init(&self->mutex);
  sys_queue_init(&self->task_queue);
}
