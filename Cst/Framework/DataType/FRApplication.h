#ifndef __FR_APPLICATION__
#define __FR_APPLICATION__

#include <Framework/Device/FRWindow.h>
#include <Framework/DataType/FRSource.h>

SYS_BEGIN_DECLS

#define FR_TYPE_APPLICATION (fr_application_get_type())
#define FR_APPLICATION(o) ((FRApplication* )sys_object_cast_check(o, FR_TYPE_APPLICATION))
#define FR_APPLICATION_CLASS(o) ((FRApplicationClass *)sys_class_cast_check(o, FR_TYPE_APPLICATION))
#define FR_APPLICATION_GET_CLASS(o) sys_instance_get_class(o, FRApplicationClass)


struct _FRApplicationClass {
  FRSourceClass parent;
};

struct _FRApplication {
  FRSource parent;
  FRApplicationPrivate *priv;
};

SYS_API SysType fr_application_get_type(void);
SYS_API FRApplication * fr_application_new_I(SysPointer app_data);

SYS_END_DECLS

#endif

