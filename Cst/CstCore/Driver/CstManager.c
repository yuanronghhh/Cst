#include <CstCore/Driver/CstManager.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstLayout.h>


SYS_DEFINE_TYPE(CstManager, cst_manager, SYS_TYPE_OBJECT);


void cst_manager_gencode(CstManager *manager, CstModule* mod) {
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

  sys_clear_pointer(&self->module_env, _sys_object_unref);
  sys_clear_pointer(&self->function_env, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_manager_parent_class)->dispose(o);
}

static void cst_manager_class_init(CstManagerClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_manager_dispose;
}
