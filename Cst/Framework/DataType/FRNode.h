#ifndef __FR_NODE__
#define __FR_NODE__

#include <Framework/FRCommon.h>

SYS_BEGIN_DECLS


#define FR_TYPE_NODE (fr_node_get_type())
#define FR_NODE(o) ((FRNode* )sys_object_cast_check(o, FR_TYPE_NODE))
#define FR_NODE_CLASS(o) ((FRNodeClass *)sys_class_cast_check(o, FR_TYPE_NODE))
#define FR_NODE_GET_CLASS(o) sys_instance_get_class(o, FRNodeClass)


struct _FRNode {
  SysObject unowned;
  SysHNode tree_node;
};

struct _FRNodeClass {
  SysObjectClass parent;
};

SYS_API SysType fr_node_get_type(void);
SYS_API FRNode* fr_node_new_I(void);

SYS_API void fr_node_set_last_child(FRNode *self, FRNode * last_child);
SYS_API FRNode * fr_node_get_last_child(FRNode *self);

SYS_API void fr_node_append(FRNode *parent, FRNode *node);
SYS_API FRNode* fr_node_insert_after (FRNode *parent, FRNode *sibling, FRNode *node);
SYS_API SysBool fr_node_has_one_child(FRNode *self);

SYS_API void fr_node_handle_bfs_r(FRNode *self, FRNodeFunc func, SysPointer user_data);
SYS_API void fr_node_handle_node_ff_r(FRNode *self, FRNodeFunc func, SysPointer user_data);
SYS_API void fr_node_handle_node_ft_r(FRNode *self, FRNodeFunc func, SysPointer user_data);

SYS_END_DECLS

#endif

