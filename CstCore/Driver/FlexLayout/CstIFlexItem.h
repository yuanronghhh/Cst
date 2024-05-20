#ifndef __CST_FLEX_ITEM__
#define __CST_FLEX_ITEM__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_I_FLEX_ITEM (cst_i_flex_item_get_type())
#define CST_I_FLEX_ITEM(o) ((CstIFlexItem* )sys_object_cast_check(o, CST_TYPE_I_FLEX_ITEM))
#define CST_I_FLEX_ITEM_GET_IFACE(o) ((CstIFlexItemInterface *)SYS_TYPE_GET_INTERFACE(o, CST_TYPE_I_FLEX_ITEM))

struct _CstIFlexItemInterface {
  SysTypeInterface parent;

  const SysChar *(*get_name) (CstIFlexItem *self);
  const FrSInt4 *(*get_padding) (CstIFlexItem *self);
  const FrBound* (*get_bound) (CstIFlexItem *self);
  SysInt (*get_width) (CstIFlexItem *self);
  SysInt (*get_direction) (CstIFlexItem *self);
  SysHArray* (*get_nodes) (CstIFlexItem *self);
};

SysType cst_i_flex_item_get_type(void);
SysInt cst_i_flex_item_get_width (CstIFlexItem *self);
const SysChar* cst_i_flex_item_get_name (CstIFlexItem *self);
const FrSInt4 *cst_i_flex_item_get_padding (CstIFlexItem *self);
const FrBound* cst_i_flex_item_get_bound (CstIFlexItem *self);
SysInt cst_i_flex_item_get_direction (CstIFlexItem *self);
SysHArray* cst_i_flex_item_get_nodes (CstIFlexItem *self);

SYS_END_DECLS

#endif

