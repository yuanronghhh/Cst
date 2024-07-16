#include <CstCore/Driver/Css/CstCssEnv.h>

#include <CstCore/Parser/CstParserCore.h>
#include <CstCore/Driver/CstNode.h>


static SysRecMutex gcss_lock;
#define GCSS_LOCK sys_rec_mutex_lock(&gcss_lock)
#define GCSS_UNLOCK sys_rec_mutex_unlock(&gcss_lock)

static FrEnv *gcss_env = NULL;

void cst_css_env_set_gcss_env(FrEnv * new_gcss_env) {
  sys_return_if_fail(new_gcss_env != NULL);

  GCSS_LOCK;
  gcss_env = new_gcss_env;
  GCSS_UNLOCK;
}

FrEnv * cst_css_env_get_gcss_env(void) {
  sys_return_val_if_fail(gcss_env != NULL, NULL);

  FrEnv * css;

  GCSS_LOCK;
  css = gcss_env;
  GCSS_UNLOCK;

  return css;
}

FrEnv *cst_css_env_new_I(FrEnv *parent) {
  SysHashTable *ht;

  if(parent == NULL) {
    GCSS_LOCK;
    parent = gcss_env;
    GCSS_UNLOCK;
  }

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
  return fr_env_new_I(ht, parent);
}

void cst_css_env_setup(void) {
  CstParser* ps;
  SysChar *buildin_css_path;

  sys_rec_mutex_init(&gcss_lock);

  buildin_css_path = CST_PROJECT_DIR"/CstCore/BuildIn/Styles/Base.cst";
  gcss_env = cst_css_env_new_I(NULL);

  ps = ast_parser_new_for_gcss(buildin_css_path);

  CstParserRContext ctx = { 0 };
  ctx.realize_func = (AstNodeFunc)ast_parser_root_gstyle_handle;
  ctx.user_data = (SysPointer)ps;

  if (ps == NULL) {
    sys_abort_N(SYS_("Failed to load base style in path: %s"), buildin_css_path);
    return;
  }

  if (!cst_parser_parse(ps, &ctx)) {
    sys_abort_N(SYS_("Failed to load base style in path: %s"), buildin_css_path);
    return;
  }

  sys_object_unref(ps);
}

void cst_css_env_teardown(void) {
  sys_assert(gcss_env != NULL && "css env should be destroyed only once.");

  sys_object_unref(gcss_env);
  sys_rec_mutex_clear(&gcss_lock);
}
