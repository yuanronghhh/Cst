#ifndef __FR_MAIN__
#define __FR_MAIN__

#include <Framework/DataType/FRSource.h>

SYS_BEGIN_DECLS

#define FR_TYPE_MAIN (fr_main_get_type())
#define FR_MAIN(o) ((FRMain* )sys_object_cast_check(o, FR_TYPE_MAIN))
#define FR_MAIN_CLASS(o) ((FRMainClass *)sys_class_cast_check(o, FR_TYPE_MAIN))
#define FR_MAIN_GET_CLASS(o) sys_instance_get_class(o, FRMainClass)


struct _FRMainClass {
  SysObjectClass parent;
};

struct _FRMain {
  SysObject parent;

  FRMainPrivate *priv;
};

SYS_API SysType fr_main_get_type (void);
SYS_API FRMain *fr_main_get_main_loop (void);
SYS_API FRMain *fr_main_new_I (void);

SYS_API void fr_main_iter_next(FRMain *self, FRSource **source);
SYS_API SysBool fr_main_is_running (FRMain *self);
SYS_API void fr_main_run (FRMain *main);
SYS_API void fr_main_stop (FRMain *self);
SYS_API void fr_main_attach (FRMain * main, FRSource * source);

SYS_API void fr_main_lock (FRMain *main);
SYS_API void fr_main_unlock (FRMain *main);

SYS_API void fr_main_setup (void);
SYS_API void fr_main_teardown (void);

SYS_END_DECLS

#endif
