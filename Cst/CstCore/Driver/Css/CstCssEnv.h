#ifndef __CST_CSS_ENV_H__
#define __CST_CSS_ENV_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

void cst_css_env_setup(void);
void cst_css_env_teardown(void);
FREnv *cst_css_env_new_I(FREnv *parent);

void cst_css_env_set_gcss_env(FREnv * gcss_env);
FREnv * cst_css_env_get_gcss_env(void);
void cst_css_gstyle_parse(AstNode *ast, CstModule *v_module);

SYS_END_DECLS

#endif
