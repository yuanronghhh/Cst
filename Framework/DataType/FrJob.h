#ifndef __FR_JOB_H__
#define __FR_JOB_H__

#include <Framework//FrCommon.h>

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
  SysQueue queue;
  SysCond cond;
  SysMutex mutex;
  FrMediaTask *task;
  FR_JOB_STATE_ENUM state;
};

SYS_API SysType fr_job_get_type(void);
SYS_API FrJob *fr_job_new(void);

SYS_API FrJob *fr_job_new_I(void);

SYS_END_DECLS

#endif
