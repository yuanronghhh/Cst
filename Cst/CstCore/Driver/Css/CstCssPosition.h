#ifndef __CST_CSS_POSITION_H__
#define __CST_CSS_POSITION_H__

#include <CstCore/Driver/Css/CstCssValue.h>

SYS_BEGIN_DECLS


#define CST_TYPE_CSS_POSITION (cst_css_position_get_type())
#define CST_CSS_POSITION(o) ((CstCssPosition* )sys_object_cast_check(o, CST_TYPE_CSS_POSITION))
#define CST_CSS_POSITION_CLASS(o) ((CstCssPositionClass *)sys_class_cast_check(o, CST_TYPE_CSS_POSITION))
#define CST_CSS_POSITION_GET_CLASS(o) sys_instance_get_class(o, CstCssPositionClass)

struct _CstCssPosition {
  CstCssValue parent;
};

struct _CstCssPositionClass {
  CstCssValueClass parent;
};

SYS_API SysType cst_css_position_get_type(void);
SYS_API CstCssValue *cst_css_position_new(void);
SYS_API CstCssValue *cst_css_position_new_I(void);

SYS_END_DECLS

#endif


