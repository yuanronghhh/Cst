#include <CstCore/Driver/Css/CstCssEnv.h>
#include <CstCore/Parser/CstParserCore.h>

static CstCssEnv *gcss_env = NULL;

SYS_DEFINE_TYPE(CstCssEnv, cst_css_env, FR_TYPE_ENV);

void cst_css_env_setup(void) {
  CstParser* ps;
  SysChar *buildin_css_path = CST_PROJECT_DIR"/Cst/CstCore/BuildIn/Styles/Base.cst";

  gcss_env = (CstCssEnv *)cst_css_env_new_I(NULL);

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

FREnv *cst_css_env_new(void) {
  return sys_object_new(CST_TYPE_CSS_ENV, NULL);
}

void cst_css_env_construct(FREnv *o, CstCssEnv *parent) {
  SysHashTable *ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);

  FR_ENV_CLASS(cst_css_env_parent_class)->construct(o, ht, FR_ENV(parent));
}

FREnv *cst_css_env_new_I(CstCssEnv *parent) {
  FREnv *o = cst_css_env_new();

  cst_css_env_construct(o, parent);

  return o;
}

CstCssEnv* cst_css_env_get_gcss(void) {
  return gcss_env;
}

CstCssGroup *cst_css_env_get(CstCssEnv *self, const SysChar *key) {
  self = self ? self : gcss_env;

  return fr_env_get(FR_ENV(self), key);
}

CstCssGroup *cst_css_env_get_r(CstCssEnv *self, const SysChar *key) {
  self = self ? self : gcss_env;

  return fr_env_get_r(FR_ENV(self), key);
}

static void cst_css_env_dispose(SysObject *o) {

  SYS_OBJECT_CLASS(cst_css_env_parent_class)->dispose(o);
}

static void cst_css_env_class_init(CstCssEnvClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_css_env_dispose;
}

static void cst_css_env_init(CstCssEnv *self) {
}
