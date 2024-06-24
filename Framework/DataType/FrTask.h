#ifndef __FR_TASK_H__
#define __FR_TASK_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_TASK (fr_task_get_type())
#define FR_TASK(o) ((FrTask* )sys_object_cast_check(o, FR_TYPE_TASK))
#define FR_TASK_CLASS(o) ((FrTaskClass *)sys_class_cast_check(o, FR_TYPE_TASK))
#define FR_TASK_GET_CLASS(o) sys_instance_get_class(o, FrTaskClass)

struct _FrTaskContext {
  SysPointer user_data;
  SysPointer result;
  FrTaskFunc handler;
  FrTaskFunc callback;
};

struct _FrTaskClass {
  SysObjectClass parent;

  void (*construct) (FrTask *self, FrTaskContext *info);
};

struct _FrTask {
  SysObject parent;

  /* <private> */
  SysPointer user_data;
  SysCond cond;
  SysMutex mutex;
  SysBool is_sync;

  SysBool done;
  struct {
    SysPointer m_pointer;
    SysInt m_int;
    SysBool m_bool;
  } result;
  FrTaskFunc handler;
  FrTaskFunc callback;
};

SYS_API SysType fr_task_get_type(void);
SYS_API FrTask *fr_task_new(void);
SYS_API void fr_task_setup(void);
SYS_API void fr_task_teardown(void);

SYS_API FrTask *fr_task_new_I(FrTaskContext *info);
SYS_API FrTask *fr_task_new_handler(FrTaskFunc handler,
    SysPointer user_data);

SYS_API void fr_task_run(FrTask *self);
SYS_API void fr_task_run_async(FrTask *self);
SYS_API void fr_task_wait(FrTask *self);
SYS_API void fr_task_return_int(FrTask *self, SysInt result);
SYS_API void fr_task_return(FrTask *self, SysInt result);
#define fr_task_create(o) sys_object_create(o, FR_TYPE_TASK)

SYS_END_DECLS

#endif
