#include <CstCore/Driver/CstManager.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstBoxNode.h>

#define MANAGER_LOCK
#define MANAGER_UNLOCK

struct _CstManagerPrivate {
  FREnv* module_env;
  SysHashTable* meta_ht;
  FREnv* function_env;
};

SYS_DEFINE_TYPE(CstManager, cst_manager, SYS_TYPE_OBJECT);

static void cst_manager_meta_setup(CstManager *manager) {
  cst_manager_set_meta(manager, "LBox", CST_TYPE_NODE);
  cst_manager_set_meta(manager, "LDiv", CST_TYPE_NODE);
  cst_manager_set_meta(manager, "LGrid", CST_TYPE_NODE);
  cst_manager_set_meta(manager, "LBody", CST_TYPE_NODE);

  cst_manager_set_meta(manager, "Text", CST_TYPE_TEXT);
}

void cst_manager_lock(CstManager *manager) {
  MANAGER_LOCK
}

void cst_manager_unlock(CstManager *manager) {
  MANAGER_UNLOCK
}

CstModule* cst_manager_load_module(CstManager *self, CstModule* parent, const SysChar* path) {
  sys_return_val_if_fail(path != NULL, NULL);

  CstModule *mod, *old;

  old = (CstModule *)fr_env_get(self->module_env, (SysPointer)path);

  if(old != NULL) {
    if(!cst_module_is_loaded(old)) {
      sys_error_N("module load cycle in %s: %s",
          cst_module_get_path(parent),
          cst_module_get_path(old));
      return NULL;
    } else {
      return old;
    }
  }

  mod = cst_module_new_I(self, parent, path);
  if (mod == NULL) {
    return NULL;
  }

  fr_env_set(self->module_env, cst_module_get_path(mod), (SysPointer)mod);

  if (!cst_module_load(mod)) {
    sys_abort_N("module parse failed: %s", path);
    return NULL;
  }

  return mod;
}

void cst_manager_gencode(CstManager *manager, CstModule* mod) {
}

void cst_manager_realize(CstManager *self, CstModule* v_module, CstRender *v_render) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_module != NULL);

  CstComponent *v_component = NULL;
  FRWindow *window = cst_render_get_default_window(v_render);
  CstNode* body = cst_render_get_root(v_render);

  CstNodeProps props = { 0 };

  cst_node_construct(v_module, v_component, NULL, body, &props);
  cst_node_realize(v_module, NULL, NULL, body, v_render);

  cst_module_realize(v_module, body, v_render);
}

CstManager *cst_manager_new(void) {
  return sys_object_new(CST_TYPE_MANAGER, NULL);
}

void cst_manager_set_function(CstManager *self, const SysChar *name, SysPointer func) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(name != NULL);

  fr_env_set(self->function_env, sys_strdup(name), func);
}

SysPointer cst_manager_get_function(CstManager *self, const SysChar *funcname) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(funcname != NULL, NULL);

  return fr_env_get(self->function_env, funcname);
}

void cst_manager_set_meta(CstManager *self, const SysChar *name, SysType stype) {
  sys_return_if_fail(self != NULL);

  sys_hash_table_insert(self->meta_ht, (SysPointer)sys_strdup(name), (SysPointer)stype);
}

SysType cst_manager_get_meta(CstManager *self, const SysChar *name) {
  sys_return_val_if_fail(self != NULL, 0);
  sys_return_val_if_fail(name != NULL, 0);

  return (SysType)sys_hash_table_lookup(self->meta_ht, (SysPointer)name);
}

/* object api */
void cst_manager_construct(CstManager *self) {
  SysHashTable *ht;
  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
  self->module_env = fr_env_new_I(ht, NULL);

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, sys_free, NULL);
  self->meta_ht = ht;

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, sys_free, NULL);
  self->function_env = fr_env_new_I(ht, NULL);

  cst_manager_meta_setup(self);
}

CstManager* cst_manager_new_I(void) {
  CstManager* o = cst_manager_new();

  cst_manager_construct(o);

  return o;
}

static void cst_manager_init(CstManager *self) {
}

static void cst_manager_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  CstManager *self = CST_MANAGER(o);
  sys_hash_table_unref(self->meta_ht);

  sys_object_unref(self->module_env);
  sys_object_unref(self->function_env);

  SYS_OBJECT_CLASS(cst_manager_parent_class)->dispose(o);
}

static void cst_manager_class_init(CstManagerClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_manager_dispose;
}
