#ifndef __FR_JOB_H__
#define __FR_JOB_H__

#include <Framework/FrCommon.h>
#include <Framework/DataType/FrJobTask.h>

SYS_BEGIN_DECLS

#define FR_TYPE_JOB (fr_job_get_type())
#define FR_JOB(o) ((FrJob* )sys_object_cast_check(o, FR_TYPE_JOB))
#define FR_JOB_CLASS(o) ((FrJobClass *)sys_class_cast_check(o, FR_TYPE_JOB))
#define FR_JOB_GET_CLASS(o) sys_instance_get_class(o, FrJobClass)

struct _FrJobClass {
  SysObjectClass parent;
};

struct _FrJob {
  SysObject parent;

  /* <private> */
  SysThread* thread;
  SysCond cond;
  SysMutex mutex;
  SysQueue task_queue;
  FrJobTask task;
  FrJobFunc callback;
  SysPointer user_data;
  FR_JOB_STATE_ENUM state;
  SysThreadFunc func;
};

SYS_API SysType fr_job_get_type(void);
SYS_API FrJob *fr_job_new(void);

SYS_API void fr_job_lock(FrJob *self);
SYS_API void fr_job_unlock(FrJob *self);

SYS_API FrJob *fr_job_new_I(void);
SYS_API void fr_job_run_task(FrJob *self, FrJobTask *task);
SYS_API void fr_job_run_task_wait(FrJob *self, FrJobTask *task);
SYS_API void fr_job_set_callback(FrJob* self, FrJobFunc callback);
SYS_API void fr_job_stop(FrJob* self);
SYS_API void fr_job_join(FrJob *self);
SYS_API void fr_job_start(FrJob *self, const SysChar *name, SysThreadFunc func);
#define fr_job_create(o) sys_object_create(o, FR_TYPE_JOB);

SYS_END_DECLS

#endif
