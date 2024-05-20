#ifndef __CST_LDIV_H__
#define __CST_LDIV_H__

#include <CstCore/Driver/CstNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_LDIV (cst_ldiv_get_type())
#define CST_LDIV_CLASS(o) ((CstLDivClass *)sys_class_cast_check(o, CST_TYPE_LDIV))
#define CST_LDIV_GET_CLASS(o) sys_instance_get_class(o, CstLDivClass)
#define CST_LDIV(o) ((CstLDiv* )sys_object_cast_check(o, CST_TYPE_LDIV))


struct _CstLDiv {
  CstNode parent;
};

struct _CstLDivClass {
  CstNodeClass parent;
};


SYS_API SysType cst_ldiv_get_type(void);
SYS_API CstNode* cst_ldiv_new(void);

SYS_END_DECLS

#endif

