#ifndef __FR_SOURCE__
#define __FR_SOURCE__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS


#define FR_TYPE_SOURCE (fr_source_get_type())
#define FR_SOURCE(o) ((FrSource* )sys_object_cast_check(o, FR_TYPE_SOURCE))
#define FR_SOURCE_CLASS(o) ((FrSourceClass *)sys_class_cast_check(o, FR_TYPE_SOURCE))
#define FR_SOURCE_GET_CLASS(o) sys_instance_get_class(o, FrSourceClass)


struct _FrSourceClass {
  SysObjectClass parent;

  void (*construct) (FrSource *source, SysPointer user_data);
  SysBool (*check) (FrSource *source);
  SysBool (*prepare) (FrSource *source);
  SysBool (*dispatch) (FrSource *source);
  void (*finish) (FrSource *source);
};

struct _FrSource {
  SysObject parent;

  /* <private> */
  FrMain *main;
  SysPointer user_data;
};

SYS_API SysType fr_source_get_type(void);
SYS_API FrSource * fr_source_new_I(SysPointer user_data);

SYS_API SysBool fr_source_check(FrSource * source);
SYS_API void fr_source_prepare(FrSource * source);
SYS_API void fr_source_finish(FrSource * source);
SYS_API void fr_source_dispatch(FrSource * source);
void fr_source_set_main(FrSource * source, FrMain * main);

SYS_END_DECLS

#endif

