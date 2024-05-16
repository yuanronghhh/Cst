#include <Framework/Media/FrMediaTask.h>

SYS_DEFINE_TYPE(FrMediaTask, fr_media_task, SYS_TYPE_OBJECT);

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
}

void fr_media_task_init(FrMediaTask* self) {
}
