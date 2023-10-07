#ifndef __CST_CSS_GAP_H__
#define __CST_CSS_GAP_H__

#include <CstCore/Driver/Css/CstCssValue.h>

SYS_BEGIN_DECLS


#define CST_TYPE_CSS_GAP (cst_css_gap_get_type())
#define CST_CSS_GAP(o) ((CstCssGap* )sys_object_cast_check(o, CST_TYPE_CSS_GAP))
#define CST_CSS_GAP_CLASS(o) ((CstCssGapClass *)sys_class_cast_check(o, CST_TYPE_CSS_GAP))
#define CST_CSS_GAP_GET_CLASS(o) sys_instance_get_class(o, CstCssGapClass)

struct _CstCssGap {
  CstCssValue parent;
};

struct _CstCssGapClass {
  CstCssValueClass parent;
};

SYS_API SysType cst_css_gap_get_type(void);
SYS_API CstCssValue *cst_css_gap_new(void);
SYS_API CstCssValue *cst_css_gap_new_I(void);

SYS_END_DECLS

#endif


