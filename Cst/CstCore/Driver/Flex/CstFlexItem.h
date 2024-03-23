#ifndef __CST_FLEX_ITEM__
#define __CST_FLEX_ITEM__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_FLEX_ITEM (cst_flex_item_get_type())
#define CST_FLEX_ITEM(o) ((CstFlexItem* )sys_object_cast_check(o, CST_TYPE_FLEX_ITEM))
#define CST_FLEX_ITEM_GET_IFACE(o) ((CstFlexItemInterface *)SYS_TYPE_GET_INTERFACE(o, CST_TYPE_FLEX_ITEM))

struct _CstFlexItemInterface {
  SysTypeInterface parent;

  const SysChar *(*get_name) (CstFlexItem *self);
  const FrSInt4 *(*get_padding) (CstFlexItem *self);
  const FrSInt4 *(*get_bound) (CstFlexItem *self);
  SysInt (*get_width) (CstFlexItem *self);
  SysInt (*get_direction) (CstFlexItem *self);
};

SysType cst_flex_item_get_type(void);
SysInt cst_flex_item_get_width (CstFlexItem *item);
const SysChar* cst_flex_item_get_name (CstFlexItem *item);
const FrSInt4 *cst_flex_item_get_padding (CstFlexItem *item);
const FrSInt4 *cst_flex_item_get_bound (CstFlexItem *item);
SysInt cst_flex_item_get_direction (CstFlexItem *item);

SYS_END_DECLS

#endif

