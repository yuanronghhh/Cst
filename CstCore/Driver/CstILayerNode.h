#ifndef __CST_I_LAYER_NODE__
#define __CST_I_LAYER_NODE__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_I_LAYER_NODE (cst_i_layer_node_get_type())
#define CST_I_LAYER_NODE(o) ((CstILayerNode* )sys_object_cast_check(o, CST_TYPE_I_LAYER_NODE))
#define CST_I_LAYER_NODE_GET_IFACE(o) ((CstILayerNodeInterface *)SYS_TYPE_GET_INTERFACE(o, CST_TYPE_I_LAYER_NODE))

struct _CstILayerNodeInterface {
  SysTypeInterface parent;

  void (*iterate_node) (CstLayerNode *self, CstLayerNodeFunc func, SysPointer user_data);
  CstLayerNode *(*get_children) (CstLayerNode *lnode);
  CstLayerNode *(*get_parent) (CstLayerNode *lnode);
  CstLayerNode *(*get_next) (CstLayerNode *lnode);
  CstLayer *(*get_layer) (CstLayerNode *lnode);
};

SysType cst_i_layer_node_get_type(void);
void cst_i_layer_node_iterate_node (CstLayerNode *self,
    CstLayerNodeFunc func,
    SysPointer user_data);

CstLayerNode * cst_i_layer_node_get_children (CstLayerNode *self);
CstLayerNode * cst_i_layer_node_get_parent (CstLayerNode *self);
CstLayerNode * cst_i_layer_node_get_next (CstLayerNode *self);
CstLayer * cst_i_layer_node_get_layer (CstLayerNode *self);

SYS_END_DECLS

#endif
