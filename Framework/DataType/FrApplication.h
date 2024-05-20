#ifndef __FR_APPLICATION__
#define __FR_APPLICATION__

#include <Framework/DataType/FrSource.h>

SYS_BEGIN_DECLS

#define FR_TYPE_APPLICATION (fr_application_get_type())
#define FR_APPLICATION(o) ((FrApplication* )sys_object_cast_check(o, FR_TYPE_APPLICATION))
#define FR_APPLICATION_CLASS(o) ((FrApplicationClass *)sys_class_cast_check(o, FR_TYPE_APPLICATION))
#define FR_APPLICATION_GET_CLASS(o) sys_instance_get_class(o, FrApplicationClass)


struct _FrApplicationClass {
  FrSourceClass parent;
};

struct _FrApplication {
  FrSource parent;

  /* <private> */
  SysPointer app_data;
  struct {
    SysBool running;
    SysDouble wait;
  } media;
};

SYS_API SysType fr_application_get_type(void);
SYS_API FrSource *fr_application_new_I(SysPointer app_data);

SYS_END_DECLS

#endif

