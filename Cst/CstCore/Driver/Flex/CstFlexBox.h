#ifndef __CST_FLEX_BOX_H__
#define __CST_FLEX_BOX_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_FLEX_BOX (cst_flex_box_get_type())
#define CST_FLEX_BOX(o) ((CstFlexBox* )sys_object_cast_check(o, CST_TYPE_FLEX_BOX))
#define CST_FLEX_BOX_CLASS(o) ((CstFlexBoxClass *)sys_class_cast_check(o, CST_TYPE_FLEX_BOX)
#define CST_FLEX_BOX_GET_CLASS(o) sys_instance_get_class(o, CstFlexBoxClass)

typedef struct _CstFlexBox CstFlexBox;
typedef struct _CstFlexBoxClass CstFlexBoxClass;

struct _CstFlexBoxClass {
  SysObjectClass parent;
};

struct _CstFlexBox {
  SysObject parent;

  /* <private> */
  SysInt direction;
};

SYS_API SysType cst_flex_box_get_type(void);
SYS_API CstFlexBox *cst_flex_box_new(void);

SYS_API CstFlexBox *cst_flex_box_new_I(void);

SYS_END_DECLS

#endif
