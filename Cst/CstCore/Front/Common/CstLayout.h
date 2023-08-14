#ifndef __CST_LAYOUT__
#define __CST_LAYOUT__

#include <CstCore/Front/Common/CstLayoutContext.h>


SYS_BEGIN_DECLS

#define CST_TYPE_LAYOUT (cst_layout_get_type())
#define CST_LAYOUT(o) ((CstLayout* )sys_object_cast_check(o, CST_TYPE_LAYOUT))
#define CST_LAYOUT_CLASS(o) ((CstLayoutClass *)sys_class_cast_check(o, CST_TYPE_LAYOUT))
#define CST_LAYOUT_GET_CLASS(o) sys_instance_get_class(o, CstLayoutClass)

struct _CstLayout {
  SysObject parent;

  CstLayoutPrivate *priv;
};

struct _CstLayoutClass {
  SysObjectClass parent;

  void (*construct) (CstLayout* o, CstLayoutContext *context);
};

SysType cst_layout_get_type(void);
CstLayout *cst_layout_new(void);
CstLayout *cst_layout_new_I(CstLayoutContext *context);

SYS_END_DECLS

#endif

