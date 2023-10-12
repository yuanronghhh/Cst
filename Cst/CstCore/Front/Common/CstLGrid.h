#ifndef __CST_LGRID_H__
#define __CST_LGRID_H__

#include <CstCore/Driver/CstNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_LGRID (cst_lgrid_get_type())
#define CST_LGRID_CLASS(o) ((CstLGridClass *)sys_class_cast_check(o, CST_TYPE_LGRID))
#define CST_LGRID_GET_CLASS(o) sys_instance_get_class(o, CstLGridClass)
#define CST_LGRID(o) ((CstLGrid* )sys_object_cast_check(o, CST_TYPE_LGRID))

struct _CstLGrid {
  CstNode parent;
};

struct _CstLGridClass {
  CstNodeClass parent;
};


SysType cst_lgrid_get_type(void);
CstNode* cst_lgrid_new(void);
CstNode* cst_lgrid_new_I(void);

SYS_END_DECLS

#endif

