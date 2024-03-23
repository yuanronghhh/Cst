#ifndef __FR_MAIN__
#define __FR_MAIN__

#include <Framework/DataType/FrSource.h>

SYS_BEGIN_DECLS

#define FR_TYPE_MAIN (fr_main_get_type())
#define FR_MAIN(o) ((FrMain* )sys_object_cast_check(o, FR_TYPE_MAIN))
#define FR_MAIN_CLASS(o) ((FrMainClass *)sys_class_cast_check(o, FR_TYPE_MAIN))
#define FR_MAIN_GET_CLASS(o) sys_instance_get_class(o, FrMainClass)


struct _FrMainClass {
  SysObjectClass parent;
};

struct _FrMain {
  SysObject parent;

  /* private */
  SysBool is_running;
  SysList* sources;
  SysList* current;
  SysRecMutex mutex;
};

SYS_API SysType fr_main_get_type (void);
SYS_API FrMain *fr_main_get_main_loop (void);
SYS_API FrMain *fr_main_new_I (void);

SYS_API void fr_main_iter_next(FrMain *self, FrSource **source);
SYS_API SysBool fr_main_is_running (FrMain *self);
SYS_API void fr_main_run (FrMain *main);
SYS_API void fr_main_stop (FrMain *self);
SYS_API void fr_main_attach (FrMain * main, FrSource * source);

SYS_API void fr_main_lock (FrMain *main);
SYS_API void fr_main_unlock (FrMain *main);

SYS_API void fr_main_setup (void);
SYS_API void fr_main_teardown (void);

SYS_END_DECLS

#endif
