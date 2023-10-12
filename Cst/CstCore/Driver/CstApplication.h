#ifndef __CST_APPLICATION_H__
#define __CST_APPLICATION_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_APPLICATION (cst_application_get_type())
#define CST_APPLICATION(o) ((CstApplication* )sys_object_cast_check(o, CST_TYPE_APPLICATION))
#define CST_APPLICATION_CLASS(o) ((CstApplicationClass *)sys_class_cast_check(o, CST_TYPE_APPLICATION))
#define CST_APPLICATION_GET_CLASS(o) sys_instance_get_class(o, CstApplicationClass)


struct _CstApplication {
  SysObject parent;

  /* <private> */
  CstManager *manager;
  FRMain* main_loop;
  FRMain* work_loop;
  FRSource *app_source;
  FRWorker *worker_source;
  CstRender *render;
  CstModule *main_module;
  SysInt status;
};

struct _CstApplicationClass {
  SysObjectClass parent;
};

SYS_API SysType cst_application_get_type(void);
SYS_API CstApplication * cst_application_new_I(const SysChar * appname);

SYS_API void cst_application_env_setup(void);
SYS_API void cst_application_env_teardown(void);

SYS_API SysInt cst_application_run(CstApplication * self, const SysChar * main_path);
SYS_API CstApplication * cst_application_acquire(void);
SYS_API void cst_application_stop (CstApplication *app);

SYS_END_DECLS

#endif

