#ifndef __CST_MODULE_H__
#define __CST_MODULE_H__

#include <CstCore/Parser/CstParserCore.h>

SYS_BEGIN_DECLS

#define CST_TYPE_MODULE (cst_module_get_type())
#define CST_MODULE(o) ((CstModule* )sys_object_cast_check(o, CST_TYPE_MODULE))
#define CST_MODULE_CLASS(o) ((CstModuleClass *)sys_class_cast_check(o, CST_TYPE_MODULE))
#define CST_MODULE_GET_CLASS(o) sys_instance_get_class(o, CstModuleClass)

struct _CstModuleClass {
  FREnvClass parent;
};

struct _CstModule {
  FREnv parent;

  /* <private> */
  SysInt count;
  FREnv* function_env;
  const SysChar* path;
  SysList* awatches;

  SysBool loaded;
  CstComponent* root_component;
  CstModule *pmodule;
};

SysType cst_module_get_type(void);

CstModule* cst_module_new(void);
CstModule* cst_module_new_I(CstModule *v_pmodule, const SysChar* path);

SysInt cst_module_get_hashcode(CstModule* self);
const SysChar *cst_module_get_path(CstModule* self);
SysBool cst_module_is_loaded(CstModule *self);

CstModule* cst_module_load_path(
    CstModule *parent,
    const SysChar* path);

void cst_module_setup(void);
void cst_module_teardown(void);

FREventFunc cst_module_get_event_function(CstModule *self, const SysChar *func_name);

CstLayerNode* cst_module_realize(CstModule* self, CstLayerNode* v_parent);

void cst_module_set_function(CstModule *self, const SysChar *func_name, SysFunc func);
SysFunc cst_module_get_function(CstModule *self, const SysChar *func_name);

void cst_module_set_component(CstModule * self, const SysChar * key, CstComponent * comp);
CstComponent* cst_module_get_component(CstModule *self, const SysChar *comp_name);

#define cst_module_set_root_node(m, n) cst_component_set_layout_node(cst_module_get_root_component(m), n)
#define cst_module_get_root_node(m) cst_component_get_layout_node(cst_module_get_root_component(m))

CstModule* cst_module_get_g_module(const SysChar *name);
SysBool cst_module_remove_g_module(const SysChar *name);
void cst_module_set_g_module(CstModule *m);

void cst_module_add_user_awatch(CstModule * self, 
  const SysChar *event_name, 
  const SysChar *func_name, FREventFunc func,
  SysPointer user_data);

SysList* cst_module_add_awatch(CstModule * self, FRAWatch *awatch);
void cst_module_remove_awatch(CstModule * self, SysList * awatch_link);

void cst_module_set_root_component(CstModule *self, CstComponent *comp);
CstComponent *cst_module_get_root_component(CstModule *self);

SysChar *cst_module_new_node_id(CstModule *self);

void cst_module_set_count(CstModule *self, SysInt count);
SysInt cst_module_get_count(CstModule *self);

SYS_END_DECLS

#endif

