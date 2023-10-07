#ifndef __CST_NODE_MAP__
#define __CST_NODE_MAP__

#include <CstCore/Front/CstPropMap.h>


SYS_BEGIN_DECLS

#define CST_TYPE_NODE_MAP (cst_node_map_get_type())
#define CST_NODE_MAP(o) ((CstNodeMap* )sys_object_cast_check(o, CST_TYPE_NODE_MAP))
#define CST_NODE_MAP_CLASS(o) ((CstNodeMapClass *)sys_class_cast_check(o, CST_TYPE_NODE_MAP))
#define CST_NODE_MAP_GET_CLASS(o) sys_instance_get_class(o, CstNodeMapClass)

struct _CstNodeMap {
  SysObject parent;

  /* <private> */
  CstPropMap* prop_map;
  SysType node_type;

  /* CST_TYPE_NODE_PROP */
  SysInt prop_type;
  SysChar* prop_name;
  SysValue* value;
  CstNodeMapFunc func;
};

struct _CstNodeMapClass {
  SysObjectClass parent;

  CstNodeMap *(*clone) (CstNodeMap *map);
};

SysType cst_node_map_get_type(void);
CstNodeMap* cst_node_map_new(void);
CstNodeMap * cst_node_map_new_I(CstPropMap * prop_map, SysInt prop_type, const SysChar * prop_name);
CstNodeMap* cst_node_map_clone(CstNodeMap* map);
void cst_node_map_bind(CstNodeMap * self, CstComNode * com_node, CstNode * v_node);
void cst_node_map_calc(CstNodeMap * self, CstNode * v_node);
const SysChar * cst_node_map_get_prop_name(CstNodeMap * self);

SYS_END_DECLS

#endif

