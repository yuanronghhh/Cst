#ifndef __CST_LGRID_CONTEXT_H__
#define __CST_LGRID_CONTEXT_H__

#include <CstCore/Driver/CstRenderContext.h>

SYS_BEGIN_DECLS


#define CST_TYPE_LGRID_CONTEXT (cst_lgrid_context_get_type())
#define CST_LGRID_CONTEXT_CLASS(o) ((CstLGridContextClass *)sys_class_cast_check(o, CST_TYPE_LGRID_CONTEXT))
#define CST_LGRID_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, CstLGridContextClass)
#define CST_LGRID_CONTEXT(o) ((CstLGridContext* )sys_object_cast_check(o, CST_TYPE_LGRID_CONTEXT))


struct _CstLGridContext {
  CstRenderContext parent;

  /* <private> */
  CstCssClosure* child_width_calc;
  CstCssClosure* child_height_calc;
};

struct _CstLGridContextClass {
  CstRenderContextClass parent;
};

SysType cst_lgrid_context_get_type(void);
CstRenderContext* cst_lgrid_context_new(void);
CstRenderContext* cst_lgrid_context_new_I(void);

SYS_END_DECLS

#endif

