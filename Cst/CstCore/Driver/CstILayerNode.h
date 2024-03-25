#ifndef __CST_I_LAYER_NODE__
#define __CST_I_LAYER_NODE__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_I_LAYER_NODE (cst_i_layer_node_get_type())
#define CST_I_LAYER_NODE(o) ((CstILayerNode* )sys_object_cast_check(o, CST_TYPE_I_LAYER_NODE))
#define CST_I_LAYER_NODE_GET_IFACE(o) ((CstILayerNodeInterface *)SYS_TYPE_GET_INTERFACE(o, CST_TYPE_I_LAYER_NODE))

typedef struct _CstILayerNode CstILayerNode;
typedef struct _CstILayerNodeInterface CstILayerNodeInterface;


struct _CstILayerNodeInterface {
  SysTypeInterface parent;
};

SysType cst_i_layer_node_get_type(void);


SYS_END_DECLS

#endif
