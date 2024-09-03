#include <Framework/DataType/FrJob.h>

SYS_DEFINE_TYPE(FrJob, fr_job, SYS_TYPE_OBJECT);

void fr_job_run_task_async(FrJob *self, FrTask *task) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(task != NULL);

  sys_async_queue_push(&self->queue, task);
}

void fr_job_run_task_sync(FrJob *self, FrTask *task) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(task != NULL);

  task->is_sync = true;
  task = sys_object_ref(task);
  sys_async_queue_push(&self->queue, task);
  fr_task_wait(task);
  sys_object_unref(task);
}

static SysPointer job_thread(SysPointer user_data) {
  FrJob *self = user_data;

  while (self->state == FR_JOB_STATE_RUNNING) {
    FrTask *task = sys_async_queue_pop(&self->queue);
    fr_task_run(task);
    sys_object_unref(task);
  }

  sys_debug_N("exit %s", self->name);

  return NULL;
}

static SysPointer stop_it(FrTask *task, SysPointer user_data) {
  FrJob *self = user_data;
  self->state = FR_JOB_STATE_STOP;
  return NULL;
}

void fr_job_stop(FrJob* self) {
  sys_return_if_fail(self != NULL);
  if (self->state == FR_JOB_STATE_STOP) { return; }

  FrTask *task;

  task = fr_task_new_handler(stop_it, self);
  fr_job_run_task_sync(self, task);
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
}

void fr_job_construct(FrJob *self, FrJobContext *info) {
  sys_return_if_fail(self != NULL);

  FrJobClass* cls = FR_JOB_GET_CLASS(self);
  sys_return_if_fail(cls->construct != NULL);

  cls->construct(self, info);
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

  sys_async_queue_clear_full(&self->queue);

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
  sys_async_queue_init_full(&self->queue, (SysDestroyFunc)_sys_object_unref);
}
