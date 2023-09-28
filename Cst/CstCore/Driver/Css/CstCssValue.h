#ifndef __CST_CSS_VALUE_H__
#define __CST_CSS_VALUE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


#define CST_TYPE_CSS_VALUE (cst_css_value_get_type())
#define CST_CSS_VALUE(o) ((CstCssValue* )sys_object_cast_check(o, CST_TYPE_CSS_VALUE))
#define CST_CSS_VALUE_CLASS(o) ((CstCssValueClass *)sys_class_cast_check(o, CST_TYPE_CSS_VALUE))
#define CST_CSS_VALUE_GET_CLASS(o) sys_instance_get_class(o, CstCssValueClass)

struct _CstCssValue {
  SysObject parent;

  /* < private > */
  SysChar *name;

  /* CST_CSS_PROP_ENUM */
  SysInt css_type;

  /* CST_RENDER_STATE_ENUM */
  SysInt state_flag;
};

struct _CstCssValueClass {
  SysObjectClass parent;

  SysInt (*parse) (JNode *jnode, CstCssValue *value);
  SysInt (*calc) (CstCssValue *self, CstLayoutContext *ctx, CstLayout *layout);
  CstCssValue* (*get_value) (CstCssValue *self);
  SysInt (*set_value) (CstCssValue *self);
};

SYS_API SysType cst_css_value_get_type(void);
SYS_API CstCssValue *cst_css_value_new(void);

SYS_API void cst_css_value_setup(void);
SYS_API void cst_css_value_teardown(void);

SYS_END_DECLS

#endif


