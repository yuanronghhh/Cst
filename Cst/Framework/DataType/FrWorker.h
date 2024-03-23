#ifndef __FR_WORKER__
#define __FR_WORKER__

#include <Framework/Device/FrWindow.h>
#include <Framework/DataType/FrSource.h>

SYS_BEGIN_DECLS

#define FR_TYPE_WORKER (fr_worker_get_type())
#define FR_WORKER(o) ((FrWorker* )sys_object_cast_check(o, FR_TYPE_WORKER))
#define FR_WORKER_CLASS(o) ((FrWorkerClass *)sys_class_cast_check(o, FR_TYPE_WORKER))
#define FR_WORKER_GET_CLASS(o) sys_instance_get_class(o, FrWorkerClass)


struct _FrWorkerClass {
  FrSourceClass parent;
};

struct _FrWorker {
  FrSource parent;

  /* <private> */
  SysPointer app_data;
};

SYS_API SysType fr_worker_get_type(void);
SYS_API FrSource *fr_worker_new_I(SysPointer app_data);

SYS_END_DECLS

#endif

