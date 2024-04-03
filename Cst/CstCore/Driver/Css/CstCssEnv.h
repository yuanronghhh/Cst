#ifndef __CST_CSS_ENV_H__
#define __CST_CSS_ENV_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

void cst_css_env_setup(void);
void cst_css_env_teardown(void);
FrEnv *cst_css_env_new_I(FrEnv *parent);

void cst_css_env_set_gcss_env(FrEnv * gcss_env);
FrEnv * cst_css_env_get_gcss_env(void);

SYS_END_DECLS

#endif
