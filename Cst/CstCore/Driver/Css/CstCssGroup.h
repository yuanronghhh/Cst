#ifndef __CST_CSS_GROUP_H__
#define __CST_CSS_GROUP_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


#define CST_TYPE_CSS_GROUP (cst_css_group_get_type())
#define CST_CSS_GROUP(o) ((CstCssGroup* )sys_object_cast_check(o, CST_TYPE_CSS_GROUP))
#define CST_CSS_GROUP_CLASS(o) ((CstCssGroupClass *)sys_class_cast_check(o, CST_TYPE_CSS_GROUP))
#define CST_CSS_GROUP_GET_CLASS(o) sys_instance_get_class(o, CstCssGroupClass)

struct _CstCssGroup {
  SysObject parent;

  /* < private > */
  SysChar *key;
  CstCssValue *value;
};

struct _CstCssGroupClass {
  SysObjectClass parent;
};

SYS_API SysType cst_css_group_get_type(void);
SYS_API CstCssGroup *cst_css_group_new(void);
SYS_API CstCssGroup *cst_css_group_clone(CstCssGroup *o);
SYS_API CstCssGroup *cst_css_group_new_I(SysChar *key, CstCssValue *value);

SYS_END_DECLS

#endif

