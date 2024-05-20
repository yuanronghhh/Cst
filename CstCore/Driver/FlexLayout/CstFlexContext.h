#ifndef __CST_FLEX_CONTEXT_H__
#define __CST_FLEX_CONTEXT_H__

#include <CstCore/Driver/CstRenderContext.h>

SYS_BEGIN_DECLS

#define CST_TYPE_FLEX_CONTEXT (cst_flex_context_get_type())
#define CST_FLEX_CONTEXT(o) ((CstFlexContext* )sys_object_cast_check(o, CST_TYPE_FLEX_CONTEXT))
#define CST_FLEX_CONTEXT_CLASS(o) ((CstFlexContextClass *)sys_class_cast_check(o, CST_TYPE_FLEX_CONTEXT))
#define CST_FLEX_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, CstFlexContextClass)

struct _CstFlexContextClass {
  CstRenderContextClass parent;
};

struct _CstFlexContext {
  CstRenderContext parent;

  /* <private> */
  CstFlexLine **lines;
  SysUInt n_lines;
  SysInt justify;
};

SYS_API SysType cst_flex_context_get_type(void);
SYS_API CstFlexContext *cst_flex_context_new(void);

CstFlexContext *cst_flex_context_new_I(void);

SYS_END_DECLS

#endif
