#ifndef __CST_I_COM_NODE__
#define __CST_I_COM_NODE__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_I_COM_NODE (cst_i_com_node_get_type())
#define CST_I_COM_NODE(o) ((CstIComNode* )sys_object_cast_check(o, CST_TYPE_I_COM_NODE))
#define CST_I_COM_NODE_GET_IFACE(o) ((CstIComNodeInterface *)SYS_TYPE_GET_INTERFACE(o, CST_TYPE_I_COM_NODE))

typedef struct _CstIComNode CstIComNode;
typedef struct _CstIComNodeInterface CstIComNodeInterface;


struct _CstIComNodeInterface {
  SysTypeInterface parent;

  /* <private> */
  void (*set_node_map) (CstIComNode *self, CstNodeMap* map);
};


SysType cst_i_com_node_get_type(void);

void cst_i_com_node_set_node_map (CstIComNode *self, CstNodeMap* map);


SYS_END_DECLS

#endif
