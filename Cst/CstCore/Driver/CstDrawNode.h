#ifndef __CST_DRAWNODE_H__
#define __CST_DRAWNODE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_DRAWNODE (cst_drawnode_get_type())
#define CST_DRAWNODE(o) ((CstDrawNode* )sys_object_cast_check(o, CST_TYPE_DRAWNODE)
#define CST_DRAWNODE_CLASS(o) ((CstDrawNodeClass *)sys_class_cast_check(o, CST_TYPE_DRAWNODE)
#define CST_DRAWNODE_GET_CLASS(o) sys_instance_get_class(o, CstDrawNodeClass)

typedef struct _CstDrawNode CstDrawNode;
typedef struct _CstDrawNodeClass CstDrawNodeClass;

struct _CstDrawNodeClass {
  SysObjectClass parent;
};

struct _CstDrawNode {
  CstLayoutNode parent;

  /* <private> */
};

SYS_API SysType cst_drawnode_get_type(void);
SYS_API CstDrawNode *cst_drawnode_new_I(void);

SYS_END_DECLS

#endif
