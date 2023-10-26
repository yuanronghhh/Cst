#ifndef __CST_COM_RNODE_H__
#define __CST_COM_RNODE_H__

#include <CstCore/Driver/CstNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_COM_RNODE (cst_com_rnode_get_type())
#define CST_COM_RNODE(o) ((CstComRNode* )sys_object_cast_check(o, CST_TYPE_COM_RNODE))
#define CST_COM_RNODE_CLASS(o) ((CstComRNodeClass *)sys_class_cast_check(o, CST_TYPE_COM_RNODE))
#define CST_COM_RNODE_GET_CLASS(o) sys_instance_get_class(o, CstComRNodeClass)


struct _CstComRNode {
  CstNode parent;
};

struct _CstComRNodeClass {
  CstNodeClass parent;
};

SysType cst_com_rnode_get_type(void);
CstNode* cst_com_rnode_new(void);

SYS_END_DECLS

#endif
