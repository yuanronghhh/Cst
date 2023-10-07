#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstLayer.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/Css/CstCssEnv.h>
#include <CstCore/Driver/CstManager.h>
#include <CstCore/Front/CstComponent.h>


#define MODULE_LOCK
#define MODULE_UNLOCK

struct _CstModulePrivate {
  CstManager *manager;

  SysChar* path;
  SysInt count;
  FREnv *function_env;
  SysList *awatches;

  SysBool loaded;
  CstComponent *root_component;
  AstNode* ast_node;
};

SYS_DEFINE_TYPE(CstModule, cst_module, FR_TYPE_ENV);

SysBool cst_module_for_import(SysPointer user_data, SysPtrArray *sarray, const SysChar *path) {
  sys_return_val_if_fail(path != NULL, false);

  const SysChar *id;
  CstManager *manager;
  CstModule *child;
  CstComponent *comp;

  CstModule *self = user_data;
  if (self == NULL) {
    return false;
  }

  manager = cst_module_get_manager(self);

  cst_manager_lock(manager);

  child = cst_manager_load_module(manager, self, path);
  if (child == NULL) {
    sys_error_N("import module failed: %s", path);
    abort();
  }

  if (sarray->len > 0) {
    comp = cst_module_get_root_comp(child);

    for (SysUInt i = 0; i < sarray->len; i++) {
      id = sarray->pdata[i];

      if (comp) {

        cst_module_set_comp(self, (SysPointer)id, comp);
      }
    }
  }

  cst_manager_unlock(manager);

  return true;
}

void cst_module_load_gstyle(CstModule *self, GStyle *gstyle) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(gstyle != NULL);

  cst_css_env_load_gstyle(gstyle, self->path);
}

CstManager *cst_module_get_manager(CstModule *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->manager;
}

SysBool cst_module_is_loaded(CstModule *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->loaded;
}

SysBool cst_module_realize(CstModule *self, CstNode *v_parent, CstRender *v_render) {
  sys_return_val_if_fail(self != NULL, false);

  CstComponent *comp = self->root_component;
  cst_component_realize(self, comp, v_parent, v_render);

  return true;
}

static SysBool module_parse(CstModule *self) {
  sys_return_val_if_fail(self != NULL, false);
  Parser* ps;
  ps = cst_parser_new(self->path);
  if (ps == NULL) {
    goto fail;
  }

  cst_parser_set_data(ps, self);

  if (!cst_parser_parse(ps)) {
    goto fail;
  }

  self->ast_node = cst_parser_get_root(ps);

  cst_parser_free(ps, false);
  return true;
fail:
  cst_parser_free(ps, true);
  return false;
}

CstModule* cst_module_new(void) {
  return (CstModule *)sys_object_new(CST_TYPE_MODULE, NULL);
}

SysBool cst_module_load(CstModule *self) {
  sys_return_val_if_fail(self != NULL, false);

  ast_module_parse(self->ast_node, self);

  ast_node_free(self->ast_node);

  self->loaded = true;

  return true;
}

void cst_module_set_root_comp(CstModule *self, CstComponent *comp) {
  sys_return_if_fail(self != NULL);
  self->root_component = comp;
}

CstComponent *cst_module_get_root_comp(CstModule *self) {
  sys_return_val_if_fail(self != NULL, false);
  return self->root_component;
}

SysInt cst_module_count(CstModule* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->count;
}

SysInt cst_module_count_dec(CstModule* self) {
  sys_return_val_if_fail(self != NULL, false);

  return --self->count;
}

SysInt cst_module_count_inc(CstModule* self) {
  sys_return_val_if_fail(self != NULL, false);

  return ++self->count;
}

SysInt cst_module_get_hashcode(CstModule* self) {
  sys_return_val_if_fail(self != NULL, false);

  return sys_str_hash((SysPointer)self->path);
}

SysChar* cst_module_get_path(CstModule* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->path;
}

AstNode* cst_module_get_ast_node(CstModule* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->ast_node;
}

SysFunc cst_module_get_function(CstModule *self, const SysChar *func_name) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(func_name != NULL, NULL);

  return (SysFunc)fr_env_get_r(self->function_env, func_name);
}

void cst_module_set_function(CstModule *self, const SysChar *func_name, SysFunc func) {

  sys_return_if_fail(self != NULL);
  sys_return_if_fail(func_name != NULL);
  sys_return_if_fail(sys_strneq(func_name, "F_", 2));
  sys_return_if_fail(func != NULL);

  fr_env_set(self->function_env, sys_strdup(func_name), (SysPointer)func);
}

void cst_module_set_comp(CstModule *self, const SysChar *key, CstComponent *comp) {

  sys_return_if_fail(self != NULL);
  sys_return_if_fail(comp != NULL);

  fr_env_set(FR_ENV(self), sys_strdup(key), (SysPointer)comp);
  sys_object_ref(comp);
}

CstComponent* cst_module_get_comp(CstModule *self, const SysChar *comp_name) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(comp_name != NULL, NULL);

  return fr_env_get_r(FR_ENV(self), (SysPointer)comp_name);
}

SysList* cst_module_add_awatch(CstModule *self, SysPointer user_data, const SysChar *watch_name, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(watch_name != NULL, NULL);
  sys_return_val_if_fail(func_name != NULL, NULL);
  sys_return_val_if_fail(func != NULL, NULL);
  sys_return_val_if_fail(props != NULL, NULL);

  FRAWatch *awatch = fr_awatch_new_bind(user_data, watch_name, func_name, func, props);

  self->awatches = sys_list_prepend(self->awatches, awatch);

  return self->awatches;
}

void cst_module_remove_awatch(CstModule *self, SysList *awatch_link) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(awatch_link != NULL);

  self->awatches = sys_list_delete_link(self->awatches, awatch_link);

  sys_object_unref(awatch_link->data);
}

/* object api */
static void cst_module_construct_i(FREnv* o, SysHashTable* ht, FREnv* parent) {
  FR_ENV_CLASS(cst_module_parent_class)->construct(o, ht, parent);
}

static void cst_module_construct(CstModule *self, CstManager *manager, CstModule *pmodule, const SysChar *path) {
  SysHashTable *ht;

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, sys_free, _sys_object_unref);
  cst_module_construct_i(FR_ENV(self), ht, FR_ENV(pmodule));

  self->manager = manager;
  self->path = sys_strdup(path);

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, sys_free, NULL);
  self->function_env = fr_env_new_I(ht, NULL);

  self->root_component = NULL;

  if (!module_parse(self)) {
    sys_abort_N("module parse failed: %s", path);
  }
}

CstModule* cst_module_new_I(CstManager* manager, CstModule* pmodule, const SysChar* path) {
  CstModule* o = cst_module_new();

  cst_module_construct(o, manager, pmodule, path);

  return o;
}

static void cst_module_init(CstModule *self) {
  self->loaded = false;
}

static void cst_module_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  CstModule *self = CST_MODULE(o);

  sys_clear_pointer(&self->root_component, _sys_object_unref);
  sys_clear_pointer(&self->function_env, _sys_object_unref);

  sys_list_free_full(self->awatches, _sys_object_unref);

  fr_env_set_parent(FR_ENV(self), NULL);

  sys_clear_pointer(&self->path, sys_free);

  if (!self->loaded) {
    sys_clear_pointer(&self->ast_node, ast_node_free);
  }

  SYS_OBJECT_CLASS(cst_module_parent_class)->dispose(o);
}

static void cst_module_class_init(CstModuleClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  FREnvClass* ecls = FR_ENV_CLASS(cls);

  ecls->construct = cst_module_construct_i;
  ocls->dispose = cst_module_dispose;
}
