#ifndef __CST_LCONTENT_CONTEXT_H__
#define __CST_LCONTENT_CONTEXT_H__

#include <CstCore/Driver/CstRenderContext.h>

SYS_BEGIN_DECLS


#define CST_TYPE_LCONTENT_CONTEXT (cst_lcontent_context_get_type())
#define CST_LCONTENT_CONTEXT_CLASS(o) ((CstLContentContextClass *)sys_class_cast_check(o, CST_TYPE_LCONTENT_CONTEXT))
#define CST_LCONTENT_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, CstLContentContextClass)
#define CST_LCONTENT_CONTEXT(o) ((CstLContentContext* )sys_object_cast_check(o, CST_TYPE_LCONTENT_CONTEXT))


struct _CstLContentContext {
  CstRenderContext parent;

  /* <private> */
};

struct _CstLContentContextClass {
  CstRenderContextClass parent;
};

CstRenderContext* cst_lcontent_context_new(void);
CstRenderContext* cst_lcontent_context_new_I(void);
SysType cst_lcontent_context_get_type(void);

SYS_END_DECLS

#endif

