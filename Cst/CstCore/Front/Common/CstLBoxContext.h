#ifndef __CST_LBOX_CONTEXT_H__
#define __CST_LBOX_CONTEXT_H__

#include <CstCore/Driver/CstLayoutContext.h>

SYS_BEGIN_DECLS


#define CST_TYPE_LBOX_CONTEXT (cst_lbox_context_get_type())
#define CST_LBOX_CONTEXT_CLASS(o) ((CstLBoxContextClass *)sys_class_cast_check(o, CST_TYPE_LBOX_CONTEXT))
#define CST_LBOX_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, CstLBoxContextClass)
#define CST_LBOX_CONTEXT(o) ((CstLBoxContext* )sys_object_cast_check(o, CST_TYPE_LBOX_CONTEXT))


struct _CstLBoxContext {
  CstLayoutContext parent;

  /* <private> */
  CstCssClosure* child_width_calc;
  CstCssClosure* child_height_calc;
};

struct _CstLBoxContextClass {
  CstLayoutContextClass parent;
};

CstLayoutContext* cst_lbox_context_new(void);
SysType cst_lbox_context_get_type(void);

SYS_END_DECLS

#endif

