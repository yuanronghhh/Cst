#ifndef __CST_DRAW_NODE_H__
#define __CST_DRAW_NODE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_DRAW_NODE (cst_draw_node_get_type())
#define CST_DRAW_NODE(o) ((CstDrawNode* )sys_object_cast_check(o, CST_TYPE_DRAW_NODE))
#define CST_DRAW_NODE_CLASS(o) ((CstDrawNodeClass *)sys_class_cast_check(o, CST_TYPE_DRAW_NODE)
#define CST_DRAW_NODE_GET_CLASS(o) sys_instance_get_class(o, CstDrawNodeClass)

struct _CstDrawNodeClass {
  SysObjectClass parent;
};

struct _CstDrawNode {
  SysObject parent;

  /* <private> */
};

SYS_API SysType cst_draw_node_get_type(void);
SYS_API CstDrawNode *cst_draw_node_new(void);

SYS_API CstDrawNode *cst_draw_node_new_I(void);

SYS_END_DECLS

#endif
