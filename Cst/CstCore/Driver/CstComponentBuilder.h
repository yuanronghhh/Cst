#ifndef __CST_COMPONENT_BUILDER_H__
#define __CST_COMPONENT_BUILDER_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


#define CST_TYPE_COMPONENT_BUILDER (cst_component_builder_get_type())
#define CST_COMPONENT_BUILDER(o) ((CstComponentBuilder* )sys_object_cast_check(o, CST_TYPE_COMPONENT_BUILDER))
#define CST_COMPONENT_BUILDER_CLASS(o) ((CstComponentBuilderClass *)sys_class_cast_check(o, CST_TYPE_COMPONENT_BUILDER))
#define CST_COMPONENT_BUILDER_GET_CLASS(o) sys_instance_get_class(o, CstComponentBuilderClass)


struct _CstComponentBuilder {
  SysObject parent;

  /* < private > */
  SysChar *v_id;
  SysChar *v_base_name;
};

struct _CstComponentBuilderClass {
  SysObjectClass parent;

  void (*construct) (CstComponentBuilder *o, const SysChar *v_id);
};

SysType cst_component_builder_get_type(void);
CstComponentBuilder *cst_component_builder_new(void);
CstComponentBuilder *cst_component_builder_new_I(const SysChar *v_id);
void cst_component_builder_build(CstComponentBuilder *self, CstComponent *v_component);

void cst_component_builder_set_base_name(CstComponentBuilder *self, SysChar *v_base_name);
void cst_component_builder_set_id(CstComponentBuilder *self, const SysChar *v_id);
const SysChar *cst_component_builder_get_id(CstComponentBuilder *self);

SYS_END_DECLS

#endif

