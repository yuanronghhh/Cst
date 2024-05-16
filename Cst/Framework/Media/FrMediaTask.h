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
  SysInt task_type;
  SysPointer data;
  FrMediaTaskFunc func;
};

SYS_API SysType fr_media_task_get_type(void);
SYS_API FrMediaTask *fr_media_task_new(void);

SYS_API FrMediaTask *fr_media_task_new_I(void);

SYS_END_DECLS

#endif
