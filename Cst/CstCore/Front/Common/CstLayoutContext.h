#ifndef __CST_LAYOUT_CONTEXT__
#define __CST_LAYOUT_CONTEXT__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_LAYOUT_CONTEXT (cst_layout_context_get_type())
#define CST_LAYOUT_CONTEXT(o) ((CstLayoutContext* )sys_object_cast_check(o, CST_TYPE_LAYOUT_CONTEXT))
#define CST_LAYOUT_CONTEXT_CLASS(o) ((CstLayoutContextClass *)sys_class_cast_check(o, CST_TYPE_LAYOUT_CONTEXT))
#define CST_LAYOUT_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, CstLayoutContextClass)

struct _CstLayoutContext {
  SysObject parent;

  SysInt x;
  SysInt y;
  SysInt width;
  SysInt height;
  CST_RENDER_STATE_ENUM state;

  CstLayoutContextPrivate *priv;
};

struct _CstLayoutContextClass {
  SysObjectClass parent;
};

SYS_API SysType cst_layout_context_get_type(void);
CstLayoutContext *cst_layout_context_new(void);
CstLayoutContext *cst_layout_context_new_I(SysInt x, SysInt y);

SYS_END_DECLS

#endif


