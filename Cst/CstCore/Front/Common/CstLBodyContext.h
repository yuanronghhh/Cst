#ifndef __CST_LBODY_CONTEXT_H__
#define __CST_LBODY_CONTEXT_H__

#include <CstCore/Driver/CstRenderContext.h>

SYS_BEGIN_DECLS


#define CST_TYPE_LBODY_CONTEXT (cst_lbody_context_get_type())
#define CST_LBODY_CONTEXT_CLASS(o) ((CstLBodyContextClass *)sys_class_cast_check(o, CST_TYPE_LBODY_CONTEXT))
#define CST_LBODY_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, CstLBodyContextClass)
#define CST_LBODY_CONTEXT(o) ((CstLBodyContext* )sys_object_cast_check(o, CST_TYPE_LBODY_CONTEXT))


struct _CstLBodyContext {
  CstRenderContext parent;

  /* <private> */
};

struct _CstLBodyContextClass {
  CstRenderContextClass parent;
};

CstRenderContext* cst_lbody_context_new(void);
CstRenderContext* cst_lbody_context_new_I(void);
SysType cst_lbody_context_get_type(void);

SYS_END_DECLS

#endif

