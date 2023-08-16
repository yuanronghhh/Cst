#ifndef __CST_COMPONENT__
#define __CST_COMPONENT__

#include <CstCore/Front/Common/CstNode.h>
#include <CstCore/Front/Common/CstNodeMap.h>

SYS_BEGIN_DECLS

#define CST_TYPE_COMPONENT _CST_TYPE_COMPONENT
#define CST_COMPONENT(o) _CST_COMPONENT(o)
#define CST_COMPONENT_CLASS(o) _CST_COMPONENT_CLASS(o)
#define CST_COMPONENT_GET_CLASS(o) _CST_COMPONENT_GET_CLASS(o)

struct _CstComponentProps {
  SysChar *v_id;
  SysChar *v_base_name;
};

CstComponent* cst_component_new(void);
SysType cst_component_get_type(void);
CstPropMap * cst_component_get_props_map(CstComponent * self, const SysChar * key);
void cst_component_set_props_map(CstComponent * self, CstPropMap * map);
void cst_component_realize_full(CstModule * v_module, CstComponent * self, CstNode * v_parent, CstComNode * ncomp_node, CstRender * v_render);
void cst_component_realize(CstModule * v_module, CstComponent * self, CstNode * v_parent, CstRender * v_render);
void cst_component_construct(CstComponent * self, CstModule * v_module, CstComponent * v_parent);

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

SysPointer cst_component_get_function(CstComponent *self, const SysChar *func_name);
void cst_component_set_function(CstComponent *self, const SysChar *func_name, SysPointer func);

SYS_END_DECLS

#endif

