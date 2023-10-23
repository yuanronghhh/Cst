#ifndef __CST_COMPONENT__
#define __CST_COMPONENT__

#include <CstCore/Driver/CstNodeMap.h>
#include <CstCore/Driver/CstComponentBuilder.h>

SYS_BEGIN_DECLS

#define CST_TYPE_COMPONENT (cst_component_get_type())
#define CST_COMPONENT(o) ((CstComponent* )sys_object_cast_check(o, CST_TYPE_COMPONENT))
#define CST_COMPONENT_CLASS(o) ((CstComponentClass *)sys_class_cast_check(o, CST_TYPE_COMPONENT))
#define CST_COMPONENT_GET_CLASS(o) sys_instance_get_class(o, CstComponentClass)

struct _CstComponent {
  FREnv parent;

  /* <private> */
  CstModule *v_module;
  SysChar *id;
  /* ComStyle : CstCssGroup */
  FREnv *style_env;
  /* ComProps: CstPropMap */
  FREnv *prop_maps_env;
  CstNode *layout_node;
  SysInt comp_type;
};

struct _CstComponentClass {
  FREnvClass parent;

  void (*construct) (CstComponent *self, CstComponentBuilder *builder);
};

CstComponent* cst_component_new(void);
SysType cst_component_get_type(void);
CstPropMap * cst_component_get_props_map(CstComponent * self, const SysChar * key);
void cst_component_set_props_map(CstComponent * self, CstPropMap * map);
CstRenderNode* cst_component_realize(CstModule *v_module, CstComponent *self, CstRenderNode *v_parent, CstRender *v_render);
CstRenderNode* cst_component_realize_full(CstModule *v_module, CstComponent *self, CstRenderNode *v_parent, CstComNode *ncomp_node, CstRender *v_render);
void cst_component_construct(CstComponent *self, CstComponentBuilder *builder);

const SysChar* cst_component_get_id(CstComponent* self);
void cst_component_set_id(CstComponent* self, const SysChar *id);
void cst_component_print(CstComponent * self);
CstCssGroup *cst_component_get_css_r(CstComponent* self, const SysChar *key);
CstCssGroup *cst_component_get_css(CstComponent* self, const SysChar *key);
void cst_component_set_css(CstComponent* self, CstCssGroup *g);
SysBool cst_component_remove_css(CstComponent * self, CstCssGroup * g);
FREnv *cst_component_get_css_env(CstComponent* self);
CST_COMPONENT_BODY_ENUM cst_component_body_get_by_name(const SysChar* name);
CST_COMPONENT_PROP_ENUM cst_component_prop_get_by_name(const SysChar * name);
void cst_component_set_layout_node(CstComponent *self, CstNode *node);
CstNode * cst_component_get_layout_node(CstComponent * self);

SYS_END_DECLS

#endif

