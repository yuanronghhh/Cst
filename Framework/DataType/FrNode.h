#ifndef __FR_NODE__
#define __FR_NODE__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS


#define FR_TYPE_NODE (fr_node_get_type())
#define FR_NODE(o) ((FrNode* )sys_object_cast_check(o, FR_TYPE_NODE))
#define FR_NODE_CLASS(o) ((FrNodeClass *)sys_class_cast_check(o, FR_TYPE_NODE))
#define FR_NODE_GET_CLASS(o) sys_instance_get_class(o, FrNodeClass)


struct _FrNode {
  SysObject unowned;

  SysHNode tree;
};

struct _FrNodeClass {
  SysObjectClass parent;
};

SYS_API SysType fr_node_get_type(void);
SYS_API FrNode* fr_node_new_I(void);

SYS_API void fr_node_set_last_child(FrNode *self, FrNode * last_child);
SYS_API FrNode * fr_node_get_last_child(FrNode *self);

SYS_API void fr_node_append(FrNode *parent, FrNode *node);
SYS_API FrNode* fr_node_insert_after (FrNode *parent, FrNode *sibling, FrNode *node);

SYS_API void fr_node_handle_bfs_r(FrNode *self, FrNodeFunc func, SysPointer user_data);
SYS_API void fr_node_handle_node_ff_r(FrNode *self, FrNodeFunc func, SysPointer user_data);
SYS_API void fr_node_handle_ft_r(FrNode *self, FrNodeFunc func, SysPointer user_data);

void fr_node_set_parent(FrNode *self, FrNode * parent);
FrNode * fr_node_get_parent(FrNode *self);

FrNode* fr_node_get_children(FrNode* self);
FrNode* fr_node_get_next(FrNode* self);
FrNode* fr_node_get_prev(FrNode* self);

SYS_END_DECLS

#endif

