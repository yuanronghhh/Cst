#ifndef __CST_COM_R_NODE_H__
#define __CST_COM_R_NODE_H__

#include <CstCore/Driver/CstRenderNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_COM_R_NODE (cst_com_r_node_get_type())
#define CST_COM_R_NODE(o) ((CstComRNode* )sys_object_cast_check(o, CST_TYPE_COM_R_NODE))
#define CST_COM_R_NODE_CLASS(o) ((CstComRNodeClass *)sys_class_cast_check(o, CST_TYPE_COM_R_NODE))
#define CST_COM_R_NODE_GET_CLASS(o) sys_instance_get_class(o, CstComRNodeClass)


struct _CstComRNode {
  CstRenderNode parent;
};

struct _CstComRNodeClass {
  CstRenderNodeClass parent;
};

SysType cst_com_r_node_get_type(void);
CstRenderNode* cst_com_r_node_new(void);

SYS_END_DECLS

#endif

