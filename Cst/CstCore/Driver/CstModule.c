#include <CstCore/Driver/CstModule.h>

#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstLayer.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/Css/CstCssEnv.h>
#include <CstCore/Driver/CstComponent.h>

static SysHashTable* g_module_ht;

typedef struct _CstModuleContext CstModuleContext;

struct _CstModuleContext {
  CstModule *v_module;
  CstModule *v_pmodule;
  CstNode *v_pnode;
};


SYS_DEFINE_TYPE(CstModule, cst_module, FR_TYPE_ENV);


CstModule* cst_module_load_path(
    CstModule *parent,
    const SysChar* path) {
  sys_return_val_if_fail(path != NULL, NULL);

  CstModule *mod, *old;
  CstParser *ps;
  CstParserContext* ctx;
  CstNode *pnode = cst_node_get_body_node();

  old = cst_module_get_g_module(path);
  if(old != NULL) {
    if(!old->loaded) {
      sys_error_N("module load cycle in %s: %s",
          cst_module_get_path(parent),
          cst_module_get_path(old));

      return NULL;
    } else {

      return old;
    }
  }

  mod = cst_module_new_I(parent, path);
  if (mod == NULL) {
    return NULL;
  }
  cst_module_set_g_module(mod);

  ctx = cst_parser_context_new();
  ps = ast_parser_new_I(path, mod, pnode);

  ctx->import_func = (AstNodeFunc)ast_parser_import_handle;
  ctx->realize_func = (AstNodeFunc)ast_parser_module_handle;
  ctx->user_data = ps;
  cst_parser_set_ctx(ps, ctx);

  if(!cst_parser_parse(ps)) {
    goto fail;
  }
  sys_object_unref(ps);
  mod->loaded = true;

  return mod;

fail:
  cst_module_remove_g_module(path);
  return NULL;
}

SysBool cst_module_is_loaded(CstModule *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->loaded;
}

CstModule* cst_module_new(void) {
  return (CstModule *)sys_object_new(CST_TYPE_MODULE, NULL);
}

void cst_module_set_root_component(CstModule *self, CstComponent *comp) {
  sys_return_if_fail(self != NULL);

  self->root_component = comp;
}

CstComponent *cst_module_get_root_component(CstModule *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->root_component;
}

SysInt cst_module_get_hashcode(CstModule* self) {
  sys_return_val_if_fail(self != NULL, false);

  return sys_str_hash((SysPointer)cst_module_get_path(self));
}

void cst_module_set_count(CstModule *self, SysInt count) {
  sys_return_if_fail(self != NULL);

  self->count = count;
}

SysInt cst_module_get_count(CstModule *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->count;
}

const SysChar* cst_module_get_path(CstModule* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->path;
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

void cst_module_set_component(CstModule *self, const SysChar *key, CstComponent *comp) {

  sys_return_if_fail(self != NULL);
  sys_return_if_fail(comp != NULL);

  fr_env_set(FR_ENV(self), sys_strdup(key), (SysPointer)comp);
  sys_object_ref(comp);
}

CstComponent* cst_module_get_component(CstModule *self, const SysChar *comp_name) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(comp_name != NULL, NULL);

  return fr_env_get_r(FR_ENV(self), (SysPointer)comp_name);
}

void cst_module_add_user_awatch(CstModule * self, const SysChar *event_name, const SysChar *func_name, FREventFunc func) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(event_name != NULL);
  sys_return_if_fail(func_name != NULL);
  sys_return_if_fail(func != NULL);

  FRAWatchBuilder *builder = fr_awatch_builder_new_I(func_name, func);
  FRAWatch *awatch = fr_awatch_new_by_name(event_name);
  fr_awatch_construct(awatch, builder);
  sys_object_unref(builder);

  cst_module_add_awatch(self, awatch);
}

SysList* cst_module_add_awatch(CstModule * self, FRAWatch *awatch) {
  sys_return_val_if_fail(self != NULL, NULL);

  self->awatches = sys_list_prepend(self->awatches, awatch);
  return self->awatches;
}

void cst_module_remove_awatch(CstModule *self, SysList *awatch_link) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(awatch_link != NULL);

  self->awatches = sys_list_delete_link(self->awatches, awatch_link);
  sys_object_unref(awatch_link->data);
}

SysChar *cst_module_new_uid(CstModule *self) {
  SysChar *nid = NULL;
  SysUInt mid;
  SysUInt ccount = 0;

  if (self) {
    mid = cst_module_get_hashcode(self);
    ccount = cst_module_get_count(self);

  } else {

    mid = sys_str_hash((SysPointer)"root-node");
  }

  nid = sys_strdup_printf("id.%u.%u", mid, ccount);

  return nid;
}

FREventFunc cst_module_get_event_function(CstModule *self, const SysChar *func_name) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(func_name != NULL, NULL);
  SysChar *new_func_name;
  FREventFunc func;

  new_func_name = sys_strdup_printf("%s%s", FR_FUNC_EVENT_PREFIX, func_name);
  func = (FREventFunc)cst_module_get_function(self, new_func_name);
  sys_free_N(new_func_name);

  return func;
}

SysBool cst_module_realize(CstModule *self, CstLayerNode *v_parent) {
  sys_return_val_if_fail(self != NULL, false);

  // cst_node_print_r(cst_node_get_body_node(), NULL);

  CstComponent *comp = self->root_component;
  cst_component_realize(comp, v_parent, NULL);

  return true;
}

CstModule* cst_module_get_g_module(const SysChar *name) {
  sys_return_val_if_fail(name != NULL, NULL);

  return sys_hash_table_lookup(g_module_ht, (const SysPointer)name);
}

SysBool cst_module_remove_g_module(const SysChar *name) {
  sys_return_val_if_fail(name != NULL, false);

  return sys_hash_table_remove(g_module_ht, (SysPointer)name);
}

void cst_module_set_g_module(CstModule *m) {
  sys_return_if_fail(m != NULL);

  sys_hash_table_insert(g_module_ht, (SysPointer)cst_module_get_path(m), (SysPointer)m);
}

void cst_module_setup(void) {
  sys_assert(g_module_ht == NULL);

  g_module_ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
}

void cst_module_teardown(void) {
  sys_clear_pointer(&g_module_ht, sys_hash_table_unref);
}

/* object api */
static void cst_module_construct_i(FREnv* o, SysHashTable* ht, FREnv* parent) {

  FR_ENV_CLASS(cst_module_parent_class)->construct(o, ht, parent);
}

static void cst_module_construct(CstModule *self, CstModule *pmodule, const SysChar* path) {
  SysHashTable *ht;

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, sys_free, (SysDestroyFunc)_sys_object_unref);
  self->path = path;

  cst_module_construct_i(FR_ENV(self), ht, FR_ENV(pmodule));
  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, sys_free, NULL);

  self->function_env = fr_env_new_I(ht, NULL);
  self->root_component = NULL;
  self->pmodule = pmodule;
}

CstModule* cst_module_new_I(CstModule *v_pmodule, const SysChar* path) {
  if(!sys_path_exists(path)) {
    sys_warning_N("module path not exists: %s", path);
    return NULL;
  }

  CstModule* self = cst_module_new();

  cst_module_construct(self, v_pmodule, path);

  return self;
}

static void cst_module_init(CstModule *self) {
  self->loaded = false;
}

static void cst_module_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  CstModule *self = CST_MODULE(o);

  sys_clear_pointer(&self->root_component, _sys_object_unref);
  sys_clear_pointer(&self->function_env, _sys_object_unref);

  sys_list_free_full(self->awatches, (SysDestroyFunc)_sys_object_unref);

  fr_env_set_parent(FR_ENV(self), NULL);

  SYS_OBJECT_CLASS(cst_module_parent_class)->dispose(o);
}

static void cst_module_class_init(CstModuleClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  FREnvClass* ecls = FR_ENV_CLASS(cls);

  ecls->construct = cst_module_construct_i;
  ocls->dispose = cst_module_dispose;
}
