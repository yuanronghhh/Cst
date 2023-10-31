#include <CstCore/Driver/Css/CstCssEnv.h>

#include <CstCore/Parser/CstParserCore.h>

#define CST_GCSS_LOCK
#define CST_GCSS_UNLOCK

static FREnv *gcss_env = NULL;

void cst_css_env_set_gcss_env(FREnv * new_gcss_env) {
  sys_return_if_fail(new_gcss_env != NULL);

  CST_GCSS_LOCK;
  gcss_env = new_gcss_env;
  CST_GCSS_UNLOCK;
}

FREnv * cst_css_env_get_gcss_env(void) {
  sys_return_val_if_fail(gcss_env != NULL, NULL);

  FREnv * css;

  CST_GCSS_LOCK;
  css = gcss_env;
  CST_GCSS_UNLOCK;

  return css;
}

FREnv *cst_css_env_new_I(FREnv *parent) {
  SysHashTable *ht;

  if(parent == NULL) {
    parent = gcss_env;
  }

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
  return fr_env_new_I(ht, parent);
}

void cst_css_env_setup(void) {
  CstParser* ps;
  SysChar *buildin_css_path = CST_PROJECT_DIR"/Cst/CstCore/BuildIn/Styles/Base.cst";

  gcss_env = cst_css_env_new_I(NULL);

  ps = cst_parser_new_I(buildin_css_path);
  if (ps == NULL) {
    sys_abort_N(SYS_("Failed to load base style in path: %s"), buildin_css_path);
    return;
  }

  if (!cst_parser_parse(ps)) {
    sys_abort_N(SYS_("Failed to load base style in path: %s"), buildin_css_path);
    return;
  }

  cst_parser_gstyle_parse(ps, gcss_env);

  sys_object_unref(ps);
}

void cst_css_env_teardown(void) {
  sys_assert(gcss_env != NULL && "css env should be destroyed only once.");

  sys_object_unref(gcss_env);
}
