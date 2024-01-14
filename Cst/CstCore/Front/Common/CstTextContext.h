#ifndef __CST_TEXT_CONTEXT_H__
#define __CST_TEXT_CONTEXT_H__

#include <CstCore/Driver/CstRenderContext.h>

SYS_BEGIN_DECLS


#define CST_TYPE_TEXT_CONTEXT (cst_text_context_get_type())
#define CST_TEXT_CONTEXT_CLASS(o) ((CstTextContextClass *)sys_class_cast_check(o, CST_TYPE_TEXT_CONTEXT))
#define CST_TEXT_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, CstTextContextClass)
#define CST_TEXT_CONTEXT(o) ((CstTextContext* )sys_object_cast_check(o, CST_TYPE_TEXT_CONTEXT))


struct _CstTextContext {
  CstRenderContext parent;

  /* <private> */
};

struct _CstTextContextClass {
  CstRenderContextClass parent;
};

CstRenderContext* cst_text_context_new(void);
CstRenderContext* cst_text_context_new_I(void);
SysType cst_text_context_get_type(void);

SYS_END_DECLS

#endif

