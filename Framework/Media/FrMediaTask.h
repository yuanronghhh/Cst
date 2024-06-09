#ifndef __FR_MEDIA_TASK_H__
#define __FR_MEDIA_TASK_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_MEDIA_TASK (fr_media_task_get_type())
#define FR_MEDIA_TASK(o) ((FrMediaTask* )sys_object_cast_check(o, FR_TYPE_MEDIA_TASK))
#define FR_MEDIA_TASK_CLASS(o) ((FrMediaTaskClass *)sys_class_cast_check(o, FR_TYPE_MEDIA_TASK))
#define FR_MEDIA_TASK_GET_CLASS(o) sys_instance_get_class(o, FrMediaTaskClass)

struct _FrMediaTaskClass {
  SysObjectClass parent;
};

struct _FrMediaTask {
  SysObject parent;

  /* <private> */
  SysPointer data;
  SysBool done;
  SysPointer result;
  FrMediaTaskFunc handler;
  FrMediaTaskFunc callback;
};

SYS_API SysType fr_media_task_get_type(void);
SYS_API FrMediaTask *fr_media_task_new(void);
SYS_API void fr_media_task_setup(void);
SYS_API void fr_media_task_teardown(void);

SYS_API FrMediaTask *fr_media_task_new_I(void);
void fr_media_task_run(FrMediaTask *self);
void fr_media_task_wait(FrMediaTask* self);
SysPointer fr_media_task_result(FrMediaTask *self);
#define fr_media_task_create(o) sys_object_create(o, FR_TYPE_MEDIA_TASK);

SYS_END_DECLS

#endif
