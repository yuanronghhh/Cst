#ifndef __CST_BOX_NODE_H__
#define __CST_BOX_NODE_H__

#include <CstCore/Driver/CstRenderNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_BOX_NODE (cst_box_node_get_type())
#define CST_BOX_NODE(o) ((CstBoxNode* )sys_object_cast_check(o, CST_TYPE_BOX_NODE))
#define CST_BOX_NODE_CLASS(o) ((CstBoxNodeClass *)sys_class_cast_check(o, CST_TYPE_BOX_NODE))
#define CST_BOX_NODE_GET_CLASS(o) sys_instance_get_class(o, CstBoxNodeClass)


struct _CstBoxNode {
  CstRenderNode unowned;

  /* <private> */
  CstBoxNode   *next;
  CstBoxNode   *prev;
  CstBoxNode   *parent;
  CstBoxNode   *children;

  CstBoxNode   *last_child;
};

struct _CstBoxNodeClass {
  CstRenderNodeClass parent;
};

SYS_API SysType cst_box_node_get_type(void);
SYS_API CstRenderNode* cst_box_node_new(void);
SYS_API CstRenderNode* cst_box_node_new_I(CstNode *node);

SYS_API CstBoxNode* cst_box_node_insert_after(CstBoxNode *parent, CstBoxNode *sibling, CstBoxNode *box_node);
SYS_API void cst_box_node_append(CstBoxNode *parent, CstBoxNode *box_node);
SYS_API void cst_box_node_set_last_child(CstBoxNode *self, CstBoxNode *last_child);
SYS_API CstBoxNode *cst_box_node_get_last_child(CstBoxNode *self);
SYS_API CstBoxNode *cst_box_node_children(CstBoxNode *self);
SYS_API CstBoxNode *cst_box_node_next(CstBoxNode *self);
SYS_API void cst_box_node_relayout_root(CstBoxNode *self, CstLayout *layout);
SYS_API void cst_box_node_repaint_root(CstBoxNode *self, CstLayout *layout);
SYS_API void cst_box_node_print_node_r(CstBoxNode *self);

SYS_END_DECLS

#endif
