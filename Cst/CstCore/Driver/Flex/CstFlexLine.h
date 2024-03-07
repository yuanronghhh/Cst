#ifndef __CST_FLEX_LINE_H__
#define __CST_FLEX_LINE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_FLEX_LINE (cst_flex_line_get_type())
#define CST_FLEX_LINE(o) ((CstFlexLine* )sys_object_cast_check(o, CST_TYPE_FLEX_LINE))
#define CST_FLEX_LINE_CLASS(o) ((CstFlexLineClass *)sys_class_cast_check(o, CST_TYPE_FLEX_LINE)
#define CST_FLEX_LINE_GET_CLASS(o) sys_instance_get_class(o, CstFlexLineClass)

typedef struct _CstFlexLine CstFlexLine;
typedef struct _CstFlexLineClass CstFlexLineClass;

struct _CstFlexLineClass {
  SysObjectClass parent;
};

struct _CstFlexLine {
  SysObject parent;

  /* <private> */
  SysHArray items;
  SysInt cross_size;
};

SYS_API SysType cst_flex_line_get_type(void);
SYS_API CstFlexLine *cst_flex_line_new(void);

SYS_API CstFlexLine *cst_flex_line_new_I(void);
SYS_API void cst_flex_item_add(CstFlexLine* self, CstFlexItem* item);

SYS_END_DECLS

#endif
