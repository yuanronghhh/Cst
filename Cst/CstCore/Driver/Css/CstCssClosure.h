#ifndef __CST_CSS_CLOSURE_H__
#define __CST_CSS_CLOSURE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_CSS_CLOSURE (cst_css_closure_get_type())
#define CST_CSS_CLOSURE(o) ((CstCssClosure* )sys_object_cast_check(o, CST_TYPE_CSS_CLOSURE))
#define CST_CSS_CLOSURE_CLASS(o) ((CstCssClosureClass *)sys_class_cast_check(o, CST_TYPE_CSS_CLOSURE))
#define CST_CSS_CLOSURE_GET_CLASS(o) sys_instance_get_class(o, CstCssClosureClass)

struct _CstCssClosure {
  SysObject parent;

  /* < private > */
  SysPointer data;
  CstCssFunc func;
  SysDestroyFunc free;
};

struct _CstCssClosureClass {
  SysObjectClass parent;
};

SYS_API SysType cst_css_closure_get_type(void);
SYS_API CstCssClosure *cst_css_closure_new(void);

/* CstCssClosure */
SYS_API void cst_css_closure_calc(CstCssClosure * self, CstLayout * layout, CstRenderNode * render_node);
SYS_API CstCssClosure* cst_css_closure_new_I(SysPointer data, CstCssFunc calc, SysDestroyFunc free);

SYS_END_DECLS

#endif

