#ifndef __CST_CSS_ENV_H__
#define __CST_CSS_ENV_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_CSS_ENV (cst_css_env_get_type())
#define CST_CSS_ENV(o) ((CstCssEnv* )sys_object_cast_check(o, CST_TYPE_CSS_ENV))
#define CST_CSS_ENV_CLASS(o) ((CstCssEnvClass *)sys_class_cast_check(o, CST_TYPE_CSS_ENV))
#define CST_CSS_ENV_GET_CLASS(o) sys_instance_get_class(o, CstCssEnvClass)

struct _CstCssEnvClass {
  FREnvClass parent;
};

struct _CstCssEnv {
  FREnv env;
};

SYS_API SysType cst_css_env_get_type(void);

SYS_API FREnv *cst_css_env_new(void);
SYS_API FREnv *cst_css_env_new_I(CstCssEnv *parent);
SYS_API CstCssEnv * cst_css_env_get_gcss(void);

#define cst_css_env_set(env, key, value) fr_env_set(FR_ENV(env), key, value)
#define cst_css_env_remove(env, key) fr_env_remove(FR_ENV(env), key)

CstCssGroup *cst_css_env_get(CstCssEnv *self, const SysChar *key);
CstCssGroup *cst_css_env_get_r(CstCssEnv *self, const SysChar *key);
void cst_css_env_load_gstyle(GStyle *gstyle, const SysChar *path);

SYS_API void cst_css_env_setup(void);
SYS_API void cst_css_env_teardown(void);

SYS_END_DECLS

#endif
