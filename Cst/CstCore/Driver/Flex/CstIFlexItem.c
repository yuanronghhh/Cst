#include <CstCore/Driver/Flex/CstIFlexItem.h>


SYS_DEFINE_INTERFACE(CstIFlexItem, cst_i_flex_item, SYS_TYPE_OBJECT);

/* object api */
void cst_i_flex_item_default_init(CstIFlexItemInterface* iface) {
}

SysInt cst_i_flex_item_get_width (CstIFlexItem *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return CST_I_FLEX_ITEM_GET_IFACE(self)->get_width(self);
}

const SysChar* cst_i_flex_item_get_name (CstIFlexItem *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return CST_I_FLEX_ITEM_GET_IFACE(self)->get_name(self);
}

SysInt cst_i_flex_item_get_direction (CstIFlexItem *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return CST_I_FLEX_ITEM_GET_IFACE(self)->get_direction(self);
}

const FrSInt4 *cst_i_flex_item_get_padding (CstIFlexItem *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return CST_I_FLEX_ITEM_GET_IFACE(self)->get_padding(self);
}

const FrRect* cst_i_flex_item_get_bound (CstIFlexItem *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return CST_I_FLEX_ITEM_GET_IFACE(self)->get_bound(self);
}

SysHArray* cst_i_flex_item_get_lines (CstIFlexItem *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return CST_I_FLEX_ITEM_GET_IFACE(self)->get_lines(self);
}
