#ifndef __CST_BOX_NODE_H__
#define __CST_BOX_NODE_H__

#include <CstCore/Driver/CstLayerNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_BOX_NODE (cst_box_node_get_type())
#define CST_BOX_NODE(o) ((CstBoxNode* )sys_object_cast_check(o, CST_TYPE_BOX_NODE))
#define CST_BOX_NODE_CLASS(o) ((CstBoxNodeClass *)sys_class_cast_check(o, CST_TYPE_BOX_NODE))
#define CST_BOX_NODE_GET_CLASS(o) sys_instance_get_class(o, CstBoxNodeClass)


struct _CstBoxNode {
  CstLayerNode unowned;

  CstBoxNode   *next;
  CstBoxNode   *prev;
  CstBoxNode   *parent;
  CstBoxNode   *children;

  /* <private> */
  CstBoxNode   *last_child;
};

struct _CstBoxNodeClass {
  CstLayerNodeClass parent;
};

SysType cst_box_node_get_type(void);
CstLayerNode* cst_box_node_new(void);
CstLayerNode *cst_box_node_new_I(CstRenderNode *rnode);

CstBoxNode* cst_box_node_insert_after(CstBoxNode *parent, CstBoxNode *sibling, CstBoxNode *box_node);
void cst_box_node_append(CstBoxNode *parent, CstBoxNode *box_node);
void cst_box_node_set_last_child(CstBoxNode *self, CstBoxNode *last_child);
CstBoxNode *cst_box_node_get_last_child(CstBoxNode *self);
void cst_box_node_relayout_root(CstBoxNode *self, CstLayout *layout);
void cst_box_node_repaint_root(CstBoxNode *self, CstLayout *layout);
void cst_box_node_relayout_node(CstBoxNode* self, CstLayout* layout);

SysBool cst_box_node_has_one_child(CstBoxNode* self);
void cst_box_node_bfs_handle(CstBoxNode* self, CstLayerNodeFunc func, SysPointer user_data);
void cst_box_node_print(CstBoxNode * self, SysPointer user_data);

SYS_END_DECLS

#endif
