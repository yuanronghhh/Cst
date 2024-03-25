#ifndef __CST_ABS_NODE_H__
#define __CST_ABS_NODE_H__

#include <CstCore/Driver/CstLayerNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_ABS_NODE (cst_abs_node_get_type())
#define CST_ABS_NODE(o) ((CstAbsNode* )sys_object_cast_check(o, CST_TYPE_ABS_NODE))
#define CST_ABS_NODE_CLASS(o) ((CstAbsNodeClass *)sys_class_cast_check(o, CST_TYPE_ABS_NODE)
#define CST_ABS_NODE_GET_CLASS(o) sys_instance_get_class(o, CstAbsNodeClass)

struct _CstAbsNodeClass {
  CstLayerNodeClass parent;
};

struct _CstAbsNode {
  CstLayerNode parent;

  /* <private> */
  SysInt z_index;
};

SYS_API SysType cst_abs_node_get_type(void);
SYS_API CstLayerNode* cst_abs_node_new(void);

SYS_API CstLayerNode* cst_abs_node_new_I(CstLayer *layer);

SYS_END_DECLS

#endif
