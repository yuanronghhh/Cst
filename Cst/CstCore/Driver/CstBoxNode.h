#ifndef __CST_BOX_NODE_H__
#define __CST_BOX_NODE_H__

#include <CstCore/Driver/CstLayerNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_BOX_NODE (cst_box_node_get_type())
#define CST_BOX_NODE(o) ((CstBoxNode* )sys_object_cast_check(o, CST_TYPE_BOX_NODE))
#define CST_BOX_NODE_CLASS(o) ((CstBoxNodeClass *)sys_class_cast_check(o, CST_TYPE_BOX_NODE))
#define CST_BOX_NODE_GET_CLASS(o) sys_instance_get_class(o, CstBoxNodeClass)


struct _CstBoxNode {
  CstLayerNode parent;

  /* <private> */
  SysHNode tree_node;
};

struct _CstBoxNodeClass {
  CstLayerNodeClass parent;
};

SysType cst_box_node_get_type(void);
CstLayerNode* cst_box_node_new(void);
CstLayerNode* cst_box_node_new_I(CstNode* node);

CstBoxNode* cst_box_node_insert_after(CstBoxNode *parent, CstBoxNode *sibling, CstBoxNode *box_node);
void cst_box_node_append(CstBoxNode *parent, CstBoxNode *box_node);
void cst_box_node_set_last_child(CstBoxNode *self, CstBoxNode *last_child);
CstBoxNode *cst_box_node_get_last_child(CstBoxNode *self);
void cst_box_node_repaint_root(CstBoxNode *self, CstLayout *layout);

SysBool cst_box_node_has_one_child(CstBoxNode* self);
void cst_box_node_bfs_handle(CstBoxNode* self, CstLayerNodeFunc func, SysPointer user_data);
void cst_box_node_handle_r(CstBoxNode *self, CstLayerNodeFunc func, SysPointer user_data);
CstBoxNode* cst_box_node_get_parent(CstBoxNode* o);
void cst_box_node_print(CstBoxNode * self, SysPointer user_data);

CstBoxNode* cst_box_node_children(CstBoxNode *self);
CstBoxNode* cst_box_node_next(CstBoxNode *self);
CstBoxNode* cst_box_node_parent(CstBoxNode *self);

void cst_box_node_repaint_children(CstBoxNode *self, CstLayout *layout);
void cst_box_node_relayout_children(CstBoxNode *self, CstLayout *layout);

SYS_END_DECLS

#endif
