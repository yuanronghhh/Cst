#ifndef __CST_COMPONENT__
#define __CST_COMPONENT__

#include <CstCore/Driver/CstNodeMap.h>
#include <CstCore/Driver/CstComponentContext.h>

SYS_BEGIN_DECLS

#define CST_TYPE_COMPONENT (cst_component_get_type())
#define CST_COMPONENT(o) ((CstComponent* )sys_object_cast_check(o, CST_TYPE_COMPONENT))
#define CST_COMPONENT_CLASS(o) ((CstComponentClass *)sys_class_cast_check(o, CST_TYPE_COMPONENT))
#define CST_COMPONENT_GET_CLASS(o) sys_instance_get_class(o, CstComponentClass)

struct _CstComponent {
  FREnv unowned;

  /* <private> */
  SysChar *id;
  /* ComStyle : CstCssGroup */
  FREnv *css_env;
  /* ComProps: CstValueMap */
  FREnv *value_maps_env;
  CstNode *layout_node;
  SysInt comp_type;
};

struct _CstComponentClass {
  FREnvClass unowned;

  void (*construct) (CstComponent *o, CstComponentContext *c);
};

CstComponent* cst_component_new(void);
SysType cst_component_get_type(void);

void cst_component_setup(void);
void cst_component_teardown(void);

CstComponent* cst_component_get_g_component(const SysChar *name);
SysBool cst_component_remove_g_component(const SysChar *id);
void cst_component_set_g_component(CstComponent *m);

CstValueMap * cst_component_get_value_map(CstComponent * self, const SysChar * key);
void cst_component_set_value_map(CstComponent * self, CstValueMap * map);
void cst_component_construct(CstComponent *self, CstComponentContext *c);

void cst_component_print(CstComponent * self);
SysPtrArray *cst_component_parse_base(CstComponent *self, const SysChar *v_base[], SysUInt len);

CstCssGroup *cst_component_get_css_r(CstComponent* self, const SysChar *key);
CstCssGroup *cst_component_get_css(CstComponent* self, const SysChar *key);

CST_COMPONENT_BODY_ENUM cst_component_body_get_by_name(const SysChar* name);
CST_COMPONENT_PROP_ENUM cst_component_prop_get_by_name(const SysChar * name);
CstLayerNode* cst_component_realize(CstComponent *self, CstLayerNode *v_parent, CstComNode *com_node);

const SysChar* cst_component_get_id(CstComponent* self);
void cst_component_set_id(CstComponent* self, const SysChar *id);

void cst_component_set_css(CstComponent* self, CstCssGroup *g);
SysBool cst_component_remove_css(CstComponent * self, CstCssGroup * g);

void cst_component_set_value_maps_env(CstComponent *self, FREnv * value_maps_env);
FREnv * cst_component_get_value_maps_env(CstComponent *self);

void cst_component_set_css_env(CstComponent *self, FREnv * css_env);
FREnv * cst_component_get_css_env(CstComponent *self);

void cst_component_set_parent(CstComponent *self, CstComponent * parent);
CstComponent * cst_component_get_parent(CstComponent *self);

SYS_END_DECLS

#endif

