#ifndef __CST_COM_NODE_BUILDER_H__
#define __CST_COM_NODE_BUILDER_H__

#include <CstCore/Driver/CstNodeBuilder.h>

SYS_BEGIN_DECLS

#define CST_TYPE_COM_NODE_BUILDER (cst_com_node_builder_get_type())
#define CST_COM_NODE_BUILDER(o) ((CstComNodeBuilder* )sys_object_cast_check(o, CST_TYPE_COM_NODE_BUILDER))
#define CST_COM_NODE_BUILDER_CLASS(o) ((CstComNodeBuilderClass *)sys_class_cast_check(o, CST_TYPE_COM_NODE_BUILDER))
#define CST_COM_NODE_BUILDER_GET_CLASS(o) sys_instance_get_class(o, CstComNodeBuilderClass)


struct _CstComNodeBuilder {
  CstNodeBuilder parent;

  /* <private> */
  CstNodeMap* value;
};

struct _CstComNodeBuilderClass {
  CstNodeBuilderClass parent;
};

SysType cst_com_node_builder_get_type(void);
CstNodeBuilder* cst_com_node_builder_new(void);
CstNodeBuilder* cst_com_node_builder_new_I(CstModule* v_module, CstComponent* v_component, CstNode* v_pnode);

SYS_END_DECLS

#endif

