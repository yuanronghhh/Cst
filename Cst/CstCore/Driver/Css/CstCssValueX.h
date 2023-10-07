#ifndef __CST_CSS_VALUE_X_H__
#define __CST_CSS_VALUE_X_H__

#include <CstCore/Driver/Css/CstCssValue.h>

SYS_BEGIN_DECLS


#define CST_TYPE_CSS_VALUE_X (cst_css_value_x_get_type())
#define CST_CSS_VALUE_X(o) ((CstCssValueX* )sys_object_cast_check(o, CST_TYPE_CSS_VALUE_X))
#define CST_CSS_VALUE_X_CLASS(o) ((CstCssValueXClass *)sys_class_cast_check(o, CST_TYPE_CSS_VALUE_X))
#define CST_CSS_VALUE_X_GET_CLASS(o) sys_instance_get_class(o, CstCssValueXClass)

struct _CstCssValueX {
  CstCssValue parent;
};

struct _CstCssValueXClass {
  CstCssValueClass parent;
};

SYS_API SysType cst_css_value_x_get_type(void);
SYS_API CstCssValue *cst_css_value_x_new(void);
SYS_API CstCssValue *cst_css_value_x_new_I(void);

SYS_END_DECLS

#endif


