#ifndef __CST_NODE_MAP__
#define __CST_NODE_MAP__

#include <CstCore/Driver/CstValueMap.h>


SYS_BEGIN_DECLS

#define CST_TYPE_NODE_MAP (cst_node_map_get_type())
#define CST_NODE_MAP(o) ((CstNodeMap* )sys_object_cast_check(o, CST_TYPE_NODE_MAP))
#define CST_NODE_MAP_CLASS(o) ((CstNodeMapClass *)sys_class_cast_check(o, CST_TYPE_NODE_MAP))
#define CST_NODE_MAP_GET_CLASS(o) sys_instance_get_class(o, CstNodeMapClass)

struct _CstNodeMap {
  FRPair parent;

  /* <private> */
  CstValueMap* value_map;

  /* CST_NODE_PROP_ENUM */
  SysInt prop_type;

  /* CstRenderNode  */
  SysType node_type;

  CstNodeMapFunc func;
};

struct _CstNodeMapClass {
  FRPairClass parent;
};

SysType cst_node_map_get_type(void);
CstNodeMap* cst_node_map_new(void);
CstNodeMap* cst_node_map_new_I(CstValueMap *value_map, SysInt prop_type, const SysChar *prop_name, SysPointer value);
void cst_node_map_bind(CstNodeMap *self, CstComNode *com_node, CstRenderNode *rnode);

SYS_END_DECLS

#endif

