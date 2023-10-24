#ifndef __CST_LBODY_H__
#define __CST_LBODY_H__

#include <CstCore/Driver/CstRenderNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_LBODY (cst_lbody_get_type())
#define CST_LBODY_CLASS(o) ((CstLBodyClass *)sys_class_cast_check(o, CST_TYPE_LBODY))
#define CST_LBODY_GET_CLASS(o) sys_instance_get_class(o, CstLBodyClass)
#define CST_LBODY(o) ((CstLBody* )sys_object_cast_check(o, CST_TYPE_LBODY))


struct _CstLBody {
  CstRenderNode parent;
};

struct _CstLBodyClass {
  CstRenderNodeClass parent;
};

SysType cst_lbody_get_type(void);
CstRenderNode* cst_lbody_new(void);

SYS_END_DECLS

#endif

