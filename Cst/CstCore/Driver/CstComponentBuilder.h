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

  void (*build) (CstComponentBuilder *self, AstParser *c, CstNode *o);
};

SysType cst_component_builder_get_type(void);
CstComponentBuilder *cst_component_builder_new(void);

void cst_component_builder_build(CstComponentBuilder *self, CstComponent *o);

/* for ast */
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

void cst_component_builder_set_v_parent(CstComponentBuilder *self, CstComponent * v_parent);
CstComponent * cst_component_builder_get_v_parent(CstComponentBuilder *self);

SYS_END_DECLS

#endif

