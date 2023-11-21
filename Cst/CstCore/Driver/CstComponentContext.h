#ifndef __CST_COMPONENT_CONTEXT_H__
#define __CST_COMPONENT_CONTEXT_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


#define CST_TYPE_COMPONENT_CONTEXT (cst_component_context_get_type())
#define CST_COMPONENT_CONTEXT(o) ((CstComponentContext* )sys_object_cast_check(o, CST_TYPE_COMPONENT_CONTEXT))
#define CST_COMPONENT_CONTEXT_CLASS(o) ((CstComponentContextClass *)sys_class_cast_check(o, CST_TYPE_COMPONENT_CONTEXT))
#define CST_COMPONENT_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, CstComponentContextClass)


struct _CstComponentContext {
  SysObject unowned;

  /* < private > */
  CstModule *v_module;
  CstComponent *v_pcomponent;

  SysChar *id;
  CstComponent *base_component;
  FREnv *value_maps_env;
  FREnv *css_env;
  CstComponent *v_parent;
};

struct _CstComponentContextClass {
  SysObjectClass parent;

  void (*build) (CstComponentContext *self, AstParser *c, CstNode *o);
};

SysType cst_component_context_get_type(void);
CstComponentContext *cst_component_context_new(void);

void cst_component_context_build(CstComponentContext *self, CstComponent *o);

/* for ast */
CstValueMap * cst_component_context_get_value_map(CstComponentContext * self, const SysChar * key);
void cst_component_context_set_value_map(CstComponentContext * self, CstValueMap * map);

SysBool cst_component_context_parse_base(CstComponentContext *self, const SysChar *base);

void cst_component_context_set_css(CstComponentContext* self, CstCssGroup *g);
SysBool cst_component_context_remove_css(CstComponentContext * self, CstCssGroup * g);

void cst_component_context_set_id(CstComponentContext *self, const SysChar * id);
const SysChar * cst_component_context_get_id(CstComponentContext *self);

void cst_component_context_set_v_pnode(CstComponentContext *self, CstNode * v_pnode);
CstNode * cst_component_context_get_v_pnode(CstComponentContext *self);

void cst_component_context_set_css_env(CstComponentContext *self, FREnv * css_env);
FREnv * cst_component_context_get_css_env(CstComponentContext *self);

void cst_component_context_set_v_parent(CstComponentContext *self, CstComponent * v_parent);
CstComponent * cst_component_context_get_v_parent(CstComponentContext *self);

void cst_component_context_set_v_module(CstComponentContext *self, CstModule * v_module);
CstModule * cst_component_context_get_v_module(CstComponentContext *self);

void cst_component_context_set_v_pcomponent(CstComponentContext *self, CstComponent * v_pcomponent);
CstComponent * cst_component_context_get_v_pcomponent(CstComponentContext *self);

SYS_END_DECLS

#endif

