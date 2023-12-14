#ifndef __CST_NODE_BUILDER_H__
#define __CST_NODE_BUILDER_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


#define CST_TYPE_NODE_BUILDER (cst_node_builder_get_type())
#define CST_NODE_BUILDER(o) ((CstNodeBuilder* )sys_object_cast_check(o, CST_TYPE_NODE_BUILDER))
#define CST_NODE_BUILDER_CLASS(o) ((CstNodeBuilderClass *)sys_class_cast_check(o, CST_TYPE_NODE_BUILDER))
#define CST_NODE_BUILDER_GET_CLASS(o) sys_instance_get_class(o, CstNodeBuilderClass)


struct _CstNodeBuilder {
  SysObject parent;

  SysList *v_awatch_list;
  SysList *v_nodemap_list;
  SysPtrArray *v_css_list;

  const SysChar* v_name;
  const SysChar* v_id;
  const SysChar *v_tag;

  SysInt v_layer;
  SysChar *v_value;
  SysChar *v_label;
  SysInt  v_z_index;
  /* < private > */
};

struct _CstNodeBuilderClass {
  SysObjectClass parent;
};

SysType cst_node_builder_get_type(void);
CstNodeBuilder *cst_node_builder_new(void);

void cst_node_builder_node(CstNodeBuilder *self, CstNode *node);

SYS_END_DECLS

#endif

