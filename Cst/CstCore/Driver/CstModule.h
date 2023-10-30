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
  CstManager* manager;

  SysInt count;
  FREnv* function_env;
  SysList* awatches;

  SysBool loaded;
  CstComponent* root_component;
  CstParser* parser;
};

SYS_API SysType cst_module_get_type(void);

SYS_API CstModule* cst_module_new(void);
SysBool cst_module_load(CstModule *self);
SYS_API CstModule* cst_module_new_I(CstManager *manager, CstModule *pmodule, const SysChar *path);

SysInt cst_module_get_hashcode(CstModule* self);

const SysChar *cst_module_get_path(CstModule* self);
SysBool cst_module_is_loaded(CstModule *self);

CstManager *cst_module_get_manager(CstModule *self);
FREventFunc cst_module_get_event_function(CstModule *self, const SysChar *func_name);

SysBool cst_module_realize(CstModule * self, CstLayerNode * v_parent, CstLayout * layout);

void cst_module_set_function(CstModule *self, const SysChar *func_name, SysFunc func);
SysFunc cst_module_get_function(CstModule *self, const SysChar *func_name);

void cst_module_set_comp(CstModule * self, const SysChar * key, CstComponent * comp);
CstComponent* cst_module_get_comp(CstModule * self, const SysChar * comp_name);

void cst_module_add_user_awatch(CstModule * self, const SysChar *event_name, const SysChar *func_name, FREventFunc func);
SysList* cst_module_add_awatch(CstModule * self, FRAWatch *awatch);
void cst_module_remove_awatch(CstModule * self, SysList * awatch_link);

void cst_module_set_root_comp(CstModule *self, CstComponent *comp);
CstComponent *cst_module_get_root_comp(CstModule *self);

SysChar *cst_module_new_uid(CstModule *self);
SysBool cst_module_for_import(SysPointer user_data, SysPtrArray *sarray, const SysChar *path);

void cst_module_set_count(CstModule *self, SysInt count);
SysInt cst_module_get_count(CstModule *self);

SYS_END_DECLS

#endif

