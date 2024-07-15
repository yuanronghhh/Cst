#include <Framework/DataType/FrTask.h>


SYS_DEFINE_TYPE(FrTask, fr_task, SYS_TYPE_OBJECT);

static SysBool fr_task_destroy_i(SysObject* o) {
  FrTask *self = FR_TASK(o);

  self->user_data = NULL;
  self->handler = NULL;
  self->callback = NULL;

  return true;
}

void fr_task_run(FrTask *self) {
  sys_return_if_fail(self != NULL);

  if(self->handler) {

    self->handler(self, self->user_data);
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

void fr_task_run_sync(FrTask *self) {
  fr_task_run(self);
  fr_task_wait(self);
}

void fr_task_run_async(FrTask *self) {
  fr_task_run(self);
}

void fr_task_setup(void) {
}

void fr_task_teardown(void) {
}

void fr_task_construct(FrTask *self, FrTaskContext *info) {
  sys_return_if_fail(self != NULL);

  FrTaskClass* cls = FR_TASK_GET_CLASS(self);
  sys_return_if_fail(cls->construct != NULL);

  cls->construct(self, info);
}

void fr_task_wait(FrTask *self) {
  sys_mutex_lock(&self->mutex);

  while(!self->done) {

    sys_cond_wait(&self->cond, &self->mutex);
  }

  sys_mutex_unlock(&self->mutex);
}

void fr_task_return_int(FrTask *self, SysInt result) {
  sys_return_if_fail(self != NULL);

  self->result.m_int = result;
}

/* object api */
static void fr_task_construct_i(FrTask *self, FrTaskContext *info) {
  self->user_data = info->user_data;
  self->handler = info->handler;
  self->callback = info->callback;
  self->done = false;
}

FrTask* fr_task_new(void) {
  return sys_object_new(FR_TYPE_TASK, NULL);
}

FrTask *fr_task_new_handler(FrTaskFunc handler, 
    SysPointer user_data) {

  FrTaskContext info = {0};
  info.handler = handler;
  info.user_data = user_data;

  return fr_task_new_I(&info);
}

FrTask *fr_task_new_I(FrTaskContext *info) {
  FrTask *o = fr_task_new();

  fr_task_construct_i(o, info);

  return o;
}

static void fr_task_dispose(SysObject* o) {
  FrTask *self = FR_TASK(o);

  sys_cond_clear(&self->cond);
  sys_mutex_clear(&self->mutex);

  SYS_OBJECT_CLASS(fr_task_parent_class)->dispose(o);
}

static void fr_task_class_init(FrTaskClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_task_dispose;
  ocls->destroy = fr_task_destroy_i;
  cls->construct = fr_task_construct_i;
}

void fr_task_init(FrTask* self) {
  self->done = false;

  sys_cond_init(&self->cond);
  sys_mutex_init(&self->mutex);
}
