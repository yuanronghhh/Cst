#ifndef __CST_FLEX_ITEM__
#define __CST_FLEX_ITEM__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_FLEX_ITEM (cst_flex_item_get_type())
#define CST_FLEX_ITEM(o) ((CstFlexItem* )sys_object_cast_check(o, CST_TYPE_FLEX_ITEM))
#define CST_FLEX_ITEM_GET_IFACE(o) (CstFlexItemInterface *)SYS_TYPE_GET_INTERFACE(o, CstFlexItemClass)

typedef struct _CstFlexItem CstFlexItem;
typedef struct _CstFlexItemInterface CstFlexItemInterface;

struct _CstFlexItemInterface {

  void (*get_width) (CstFlexItem *item);
};

SysType cst_flex_item_get_type(void);

SYS_END_DECLS

#endif

