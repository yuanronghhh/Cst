#ifndef __CST_COMPONENT_BUILDER_H__
#define __CST_COMPONENT_BUILDER_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


#define CST_TYPE_COMPONENT_BUILDER (cst_component_builder_get_type())
#define CST_COMPONENT_BUILDER(o) ((CstComponentBuilder* )sys_object_cast_check(o, CST_TYPE_COMPONENT_BUILDER))
#define CST_COMPONENT_BUILDER_CLASS(o) ((CstComponentBuilderClass *)sys_class_cast_check(o, CST_TYPE_COMPONENT_BUILDER))
#define CST_COMPONENT_BUILDER_GET_CLASS(o) sys_instance_get_class(o, CstComponentBuilderClass)


struct _CstComponentBuilder {
  SysObject unowned;

  /* < private > */
  SysChar *id;
  SysChar *v_base_name;
  FREnv *prop_maps_env;
  FREnv *css_env;
  CstComponent *v_parent;
};

struct _CstComponentBuilderClass {
  SysObjectClass parent;
};

SysType cst_component_builder_get_type(void);
CstComponentBuilder *cst_component_builder_new(void);

/* for ast */
void cst_component_builder_parse(CstComponentBuilder *self, CstContext *c, AstComponentPass *pass);
void cst_component_builder_build(CstComponentBuilder *self, CstContext *c, CstComponent *o);

CstValueMap * cst_component_builder_get_value_map(CstComponentBuilder * self, const SysChar * key);
void cst_component_builder_set_value_map(CstComponentBuilder * self, CstValueMap * map);

void cst_component_builder_set_base_name(CstComponentBuilder *self, SysChar *v_base_name);

void cst_component_builder_set_css(CstComponentBuilder* self, CstCssGroup *g);
SysBool cst_component_builder_remove_css(CstComponentBuilder * self, CstCssGroup * g);

void cst_component_builder_set_id(CstComponentBuilder *self, const SysChar * id);
const SysChar * cst_component_builder_get_id(CstComponentBuilder *self);

void cst_component_builder_set_v_pnode(CstComponentBuilder *self, CstNode * v_pnode);
CstNode * cst_component_builder_get_v_pnode(CstComponentBuilder *self);

void cst_component_builder_set_css_env(CstComponentBuilder *self, FREnv * css_env);
FREnv * cst_component_builder_get_css_env(CstComponentBuilder *self);

CstComponentBuilder * cst_component_ast_get_builder(AstComponentPass *self);
CstContext *cst_component_ast_get_c(AstComponentPass *self);
CstModule * cst_component_ast_get_v_module(AstComponentPass *self);
CstNode * cst_component_ast_get_v_pnode(AstComponentPass *self);

SYS_END_DECLS

#endif

