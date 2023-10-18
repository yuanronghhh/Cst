#include <CstCore/Driver/CstManager.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstBoxNode.h>

SYS_DEFINE_TYPE(CstManager, cst_manager, SYS_TYPE_OBJECT);


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

  CstRenderNode* body = cst_render_render_node_new_root(v_render, v_module);

  cst_render_set_layer_root(v_render, body);
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

/* object api */
void cst_manager_construct(CstManager *self) {
  SysHashTable *ht;
  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
  self->module_env = fr_env_new_I(ht, NULL);

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, sys_free, NULL);
  self->function_env = fr_env_new_I(ht, NULL);

  sys_rec_mutex_init(&self->mlock);
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

  sys_object_unref(self->module_env);
  sys_object_unref(self->function_env);
  sys_rec_mutex_clear(&self->mlock);

  SYS_OBJECT_CLASS(cst_manager_parent_class)->dispose(o);
}

static void cst_manager_class_init(CstManagerClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_manager_dispose;
}
