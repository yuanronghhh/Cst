#ifndef __FR_JOB_TASK_H__
#define __FR_JOB_TASK_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_JOB_TASK (fr_job_task_get_type())
#define FR_JOB_TASK(o) ((FrJobTask* )sys_object_cast_check(o, FR_TYPE_JOB_TASK))
#define FR_JOB_TASK_CLASS(o) ((FrJobTaskClass *)sys_class_cast_check(o, FR_TYPE_JOB_TASK))
#define FR_JOB_TASK_GET_CLASS(o) sys_instance_get_class(o, FrJobTaskClass)

struct _FrJobTaskContext {
  SysPointer user_data;
  SysPointer result;
  FrJobTaskFunc handler;
  FrJobTaskFunc callback;
};

struct _FrJobTaskClass {
  SysObjectClass parent;

  void (*construct) (FrJobTask *self, FrJobTaskContext *info);
};

struct _FrJobTask {
  SysObject parent;

  /* <private> */
  SysPointer user_data;
  SysCond cond;
  SysMutex mutex;

  SysBool done;
  SysPointer result;
  FrJobTaskFunc handler;
  FrJobTaskFunc callback;
};

SYS_API SysType fr_job_task_get_type(void);
SYS_API FrJobTask *fr_job_task_new(void);
SYS_API void fr_job_task_setup(void);
SYS_API void fr_job_task_teardown(void);

SYS_API FrJobTask *fr_job_task_new_I(FrJobTaskContext *info);
SYS_API FrJobTask *fr_job_task_new_handler(FrJobTaskFunc handler, SysPointer user_data);
SYS_API void fr_job_task_run(FrJobTask *self);
SYS_API SysPointer fr_job_task_result(FrJobTask *self);
SYS_API void fr_job_task_wait(FrJobTask *self);
#define fr_job_task_create(o) sys_object_create(o, FR_TYPE_JOB_TASK)

SYS_END_DECLS

#endif
