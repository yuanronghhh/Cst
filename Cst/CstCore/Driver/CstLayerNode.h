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
};

struct _CstLayerNodeClass {
  SysObjectClass parent;

  void (*construct) (CstLayerNode *o, CstRenderNode *rnode);
  CstLayerNode* (*get_parent) (CstLayerNode *o);
};

SysType cst_layer_node_get_type(void);
CstLayerNode *cst_layer_node_new(void);

void cst_layer_node_set_rnode(CstLayerNode *self, CstRenderNode * rnode);
CstRenderNode * cst_layer_node_get_rnode(CstLayerNode *self);

SYS_END_DECLS

#endif

