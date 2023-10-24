#ifndef __CST_LBOX_H__
#define __CST_LBOX_H__

#include <CstCore/Driver/CstRenderNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_LBOX (cst_lbox_get_type())
#define CST_LBOX_CLASS(o) ((CstLBoxClass *)sys_class_cast_check(o, CST_TYPE_LBOX))
#define CST_LBOX_GET_CLASS(o) sys_instance_get_class(o, CstLBoxClass)
#define CST_LBOX(o) ((CstLBox* )sys_object_cast_check(o, CST_TYPE_LBOX))

struct _CstLBox {
  CstRenderNode parent;
};

struct _CstLBoxClass {
  CstRenderNodeClass parent;
};


SysType cst_lbox_get_type(void);
CstRenderNode* cst_lbox_new(void);
CstRenderNode* cst_lbox_new_I(void);

SYS_END_DECLS

#endif

