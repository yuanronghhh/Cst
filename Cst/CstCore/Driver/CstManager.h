/***
 * Manager is part of parser.
 */
#ifndef __MANAGER__
#define __MANAGER__

#include <CstCore/Front/CstFrontCore.h>

SYS_BEGIN_DECLS

#define CST_TYPE_MANAGER (cst_manager_get_type())
#define CST_MANAGER(o) ((CstManager* )sys_object_cast_check(o, CST_TYPE_MANAGER))
#define CST_MANAGER_CLASS(o) ((CstManagerClass *)sys_class_cast_check(o, CST_TYPE_MANAGER))
#define CST_MANAGER_GET_CLASS(o) sys_instance_get_class(o, CstManagerClass)

struct _CstManagerClass {
  FREnvClass parent;
};

struct _CstManager {
  FREnv parent;
  CstManagerPrivate *priv;
};

SYS_API SysType cst_manager_get_type(void);
SYS_API CstManager* cst_manager_new(void);

SYS_API void cst_manager_lock(CstManager *manager);
SYS_API void cst_manager_unlock(CstManager *manager);

SYS_API void cst_manager_set_meta(CstManager* manager, const SysChar *name, SysType stype);
SYS_API SysType cst_manager_get_meta(CstManager* manager, const SysChar *name);

CstManager * cst_manager_new_I(void);

SYS_API void cst_manager_set_function(CstManager* manager, const SysChar *name, SysPointer func);
SYS_API SysPointer cst_manager_get_function(CstManager* manager, const SysChar *funcname);
SYS_API CstModule* cst_manager_load_module(CstManager* manager, CstModule* parent, const SysChar* path);

void cst_manager_gencode(CstManager *manager, CstModule* mod);
void cst_manager_realize(CstManager * self, CstModule * v_module, CstRender * v_render);
void cst_manager_set_root(CstManager * self, CstModule * v_module);

SYS_END_DECLS

#endif
