#ifndef __CST_CLFLOW_H__
#define __CST_CLFLOW_H__

#include <CstCore/Driver/CstRenderContext.h>

SYS_BEGIN_DECLS

#define CST_TYPE_LFLOW (cst_lflow_get_type())
#define CST_LFLOW_CLASS(cls) ((CstLFlowClass *)cls)
#define CST_LFLOW_GET_CLASS(o) sys_instance_get_class(o, CstLFlowClass)
#define CST_LFLOW(o) ((CstLFlow* )o)

struct _CstLFlow {
  CstRenderContext parent;
  CstLFlowPrivate* priv;
};

struct _CstLFlowClass {
  CstRenderContextClass parent;
};

SYS_API SysType cst_lflow_get_type(void);
SYS_API CstNode* cst_lflow_new(void);

SYS_END_DECLS

#endif

