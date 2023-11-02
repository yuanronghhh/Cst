#include <CstCore/Driver/CstModule.h>

#include <CstCore/Driver/CstLayer.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/Css/CstCssEnv.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstNodeRealizer.h>

struct _AstModulePass {
  CstContext *c;
  CstModule  *v_module;
  CstNode *v_pnode;
};

static SysHashTable* g_module_ht;

SYS_DEFINE_TYPE(CstModule, cst_module, FR_TYPE_ENV);

CstModule* cst_module_load_path(CstContext *c, CstModule* parent, const SysChar* path) {
  sys_return_val_if_fail(path != NULL, NULL);
  CstModule *mod, *old;

  old = cst_module_get_g_module(path);

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

  mod = cst_module_new_I(parent, path);
  if (mod == NULL) {
    return NULL;
  }
  cst_module_set_g_module(mod);

  if (!cst_module_load(mod, c)) {
    sys_abort_N("module parse failed: %s", path);
    return NULL;
  }

  return mod;
}

CstModule * cst_module_ast_get_v_module(AstModulePass *c) {
  sys_return_val_if_fail(c != NULL, NULL);

  return c->v_module;
}

SysBool cst_module_for_import(AstModulePass *pass, SysPtrArray *sarray, const SysChar *path) {
  sys_return_val_if_fail(pass != NULL, false);
  const SysChar *id;
  CstModule *child;
  CstComponent *comp;

  CstContext *c = pass->c;
  if (c == NULL) { return false; }

  CstModule *self = pass->v_module;
  if (self == NULL) { return false; }

  child = cst_module_load_path(c, self, path);
  if (child == NULL) {
    sys_error_N("import module failed: %s", path);
    abort();
  }

  if (sarray->len > 0) {
    comp = cst_module_get_root_component(child);

    for (SysUInt i = 0; i < sarray->len; i++) {
      id = sarray->pdata[i];

      if (comp) {

        cst_module_set_component(self, (SysPointer)id, comp);
      }
    }
  }

  return true;
}

SysBool cst_module_is_loaded(CstModule *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->loaded;
}

CstModule* cst_module_new(void) {
  return (CstModule *)sys_object_new(CST_TYPE_MODULE, NULL);
}

SysBool cst_module_load(CstModule *self, CstContext *c) {
  sys_return_val_if_fail(self != NULL, false);
  AstModulePass pass = {0};

  pass.v_module = self;
  pass.c = c;

  cst_parser_set_user_data(self->parser, &pass);
  cst_parser_set_import_func(self->parser, (AstNodeFunc)ast_import_parse);
  cst_parser_set_realize_func(self->parser, (AstNodeFunc)ast_module_parse);

  if (!cst_parser_parse(self->parser)) {

    sys_clear_pointer(&self->parser, _sys_object_unref);
  }

  self->loaded = true;

  return true;
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

  return cst_parser_get_filename(self->parser);
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

  CstComponent *comp = self->root_component;
  cst_component_realize(comp, v_parent, NULL);

  return true;
}

CstModule* cst_module_get_g_module(const SysChar *name) {
  sys_return_val_if_fail(name != NULL, NULL);

  return sys_hash_table_lookup(g_module_ht, (const SysPointer)name);
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

static void cst_module_construct(CstModule *self, CstModule *pmodule, CstParser *ps) {
  SysHashTable *ht;

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, sys_free, (SysDestroyFunc)_sys_object_unref);
  cst_module_construct_i(FR_ENV(self), ht, FR_ENV(pmodule));

  self->parser = ps;
  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, sys_free, NULL);
  self->function_env = fr_env_new_I(ht, NULL);

  self->root_component = NULL;
}

CstModule* cst_module_new_I(CstModule* pmodule, const SysChar* path) {
  CstModule* self = cst_module_new();
  CstParser* ps = cst_parser_new_I(path);

  cst_module_construct(self, pmodule, ps);

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

  sys_clear_pointer(&self->parser, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_module_parent_class)->dispose(o);
}

static void cst_module_class_init(CstModuleClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  FREnvClass* ecls = FR_ENV_CLASS(cls);

  ecls->construct = cst_module_construct_i;
  ocls->dispose = cst_module_dispose;
}
