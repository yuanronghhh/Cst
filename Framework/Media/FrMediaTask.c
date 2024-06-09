#include <Framework/Media/FrMediaTask.h>

static SysCond pool_cond;
static SysMutex pool_mutex;

SYS_DEFINE_TYPE(FrMediaTask, fr_media_task, SYS_TYPE_OBJECT);

static SysBool fr_media_task_destroy_i(SysObject* o) {
  FrMediaTask *self = FR_MEDIA_TASK(o);

  self->result = NULL;
  self->data = NULL;
  self->handler = NULL;
  self->callback = NULL;

  return true;
}

void fr_media_task_wait(FrMediaTask* self) {
  sys_mutex_lock(&pool_mutex);

  while(!self->done) {

    sys_cond_wait(&pool_cond, &pool_mutex);
  }

  sys_mutex_unlock(&pool_mutex);
}

void fr_media_task_run(FrMediaTask *self) {
  sys_return_if_fail(self != NULL);
  sys_mutex_lock(&pool_mutex);

  self->done = false;
  if(self->handler) {

    self->result = self->handler(self, self->data);
  }

  if(self->callback) {

    self->callback(self, self->data);
  }
  self->done = true;

  sys_cond_signal(&pool_cond);
  sys_mutex_unlock(&pool_mutex);
}

void fr_media_task_setup(void) {
  sys_cond_init(&pool_cond);
  sys_mutex_init(&pool_mutex);
}

void fr_media_task_teardown(void) {
  sys_cond_clear(&pool_cond);
  sys_mutex_clear(&pool_mutex);
}

SysPointer fr_media_task_result(FrMediaTask *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->result;
}

/* object api */
static void fr_media_task_construct_i(FrMediaTask *self) {

}

FrMediaTask* fr_media_task_new(void) {
  return sys_object_new(FR_TYPE_MEDIA_TASK, NULL);
}

FrMediaTask *fr_media_task_new_I(void) {
  FrMediaTask *o = fr_media_task_new();

  fr_media_task_construct_i(o);

  return o;
}

static void fr_media_task_dispose(SysObject* o) {
  FrMediaTask *self = FR_MEDIA_TASK(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(fr_media_task_parent_class)->dispose(o);
}

static void fr_media_task_class_init(FrMediaTaskClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_media_task_dispose;
  ocls->destroy = fr_media_task_destroy_i;
}

void fr_media_task_init(FrMediaTask* self) {
}
