#ifndef __FR_WORKER__
#define __FR_WORKER__

#include <Framework/Device/FRWindow.h>
#include <Framework/DataType/FRSource.h>

SYS_BEGIN_DECLS

#define FR_TYPE_WORKER (fr_worker_get_type())
#define FR_WORKER(o) ((FRWorker* )sys_object_cast_check(o, FR_TYPE_WORKER))
#define FR_WORKER_CLASS(o) ((FRWorkerClass *)sys_class_cast_check(o, FR_TYPE_WORKER))
#define FR_WORKER_GET_CLASS(o) sys_instance_get_class(o, FRWorkerClass)


struct _FRWorkerClass {
  FRSourceClass parent;
};

struct _FRWorker {
  FRSource parent;

  /* <private> */
  SysPointer app_data;
};

SYS_API SysType fr_worker_get_type(void);
SYS_API FRSource *fr_worker_new_I(SysPointer app_data);

SYS_END_DECLS

#endif

