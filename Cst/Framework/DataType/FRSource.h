#ifndef __FR_SOURCE__
#define __FR_SOURCE__

#include <Framework/FRCommon.h>

SYS_BEGIN_DECLS


#define FR_TYPE_SOURCE (fr_source_get_type())
#define FR_SOURCE(o) ((FRSource* )sys_object_cast_check(o, FR_TYPE_SOURCE))
#define FR_SOURCE_CLASS(o) ((FRSourceClass *)sys_class_cast_check(o, FR_TYPE_SOURCE))
#define FR_SOURCE_GET_CLASS(o) sys_instance_get_class(o, FRSourceClass)


struct _FRSourceClass {
  SysObjectClass parent;

  void (*construct) (FRSource *source, SysPointer user_data);
  SysBool (*check) (FRSource *source);
  SysBool (*prepare) (FRSource *source);
  SysBool (*dispatch) (FRSource *source);
  void (*finish) (FRSource *source);
};

struct _FRSource {
  SysObject parent;

  FRSourcePrivate *priv;
};

SYS_API SysType fr_source_get_type(void);
SYS_API FRSource * fr_source_new_I(SysPointer user_data);

SYS_API SysBool fr_source_check(FRSource * source);
SYS_API void fr_source_prepare(FRSource * source);
SYS_API void fr_source_finish(FRSource * source);
SYS_API void fr_source_dispatch(FRSource * source);
void fr_source_set_main(FRSource * source, FRMain * main);

SYS_END_DECLS

#endif

