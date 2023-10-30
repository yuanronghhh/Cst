#ifndef __CST_LAYER_NODE_H__
#define __CST_LAYER_NODE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


#define CST_TYPE_LAYER_NODE (cst_layer_node_get_type())
#define CST_LAYER_NODE(o) ((CstLayerNode* )sys_object_cast_check(o, CST_TYPE_LAYER_NODE))
#define CST_LAYER_NODE_CLASS(o) ((CstLayerNodeClass *)sys_class_cast_check(o, CST_TYPE_LAYER_NODE))
#define CST_LAYER_NODE_GET_CLASS(o) sys_instance_get_class(o, CstLayerNodeClass)

struct _CstLayerNode {
  SysObject parent;

  /* < private > */
  CstRenderNode *rnode;
  CstNode *node;
};

struct _CstLayerNodeClass {
  SysObjectClass parent;

  void (*construct) (CstLayerNode *o, CstNode *rnode);
};

SysType cst_layer_node_get_type(void);
CstLayerNode *cst_layer_node_new(void);
CstLayerNode *cst_layer_node_new_I(CstNode *node);

void cst_layer_node_set_rnode(CstLayerNode *self, CstRenderNode * rnode);
CstRenderNode * cst_layer_node_get_rnode(CstLayerNode *self);

SYS_END_DECLS

#endif

