#ifndef __CST_ILAYER_NODE__
#define __CST_ILAYER_NODE__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_ILAYER_NODE (cst_ilayer_node_get_type())
#define CST_ILAYER_NODE(o) ((CstILayerNode* )sys_object_cast_check(o, CST_TYPE_ILAYER_NODE))
#define CST_ILAYER_NODE_GET_IFACE(o) ((CstILayerNodeInterface *)SYS_TYPE_GET_INTERFACE(o, CST_TYPE_ILAYER_NODE))

struct _CstILayerNodeInterface {
  SysTypeInterface parent;
};

SysType cst_ilayer_node_get_type(void);

SYS_END_DECLS

#endif

