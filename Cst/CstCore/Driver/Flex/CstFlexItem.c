#include <CstCore/Driver/Flex/CstFlexItem.h>


SYS_DEFINE_INTERFACE(CstFlexItem, cst_flex_item, SYS_TYPE_OBJECT);

/* object api */
void cst_flex_item_default_init(CstFlexItemInterface* iface) {
}

SysInt cst_flex_item_get_width (CstFlexItem *item) {
  sys_return_val_if_fail(item != NULL, -1);

  return CST_FLEX_ITEM_GET_IFACE(item)->get_width(item);
}

const SysChar* cst_flex_item_get_name (CstFlexItem *item) {
  sys_return_val_if_fail(item != NULL, NULL);

  return CST_FLEX_ITEM_GET_IFACE(item)->get_name(item);
}

SysInt cst_flex_item_get_direction (CstFlexItem *item) {
  sys_return_val_if_fail(item != NULL, -1);

  return CST_FLEX_ITEM_GET_IFACE(item)->get_direction(item);
}

const FrSInt4 *cst_flex_item_get_padding (CstFlexItem *item) {
  sys_return_val_if_fail(item != NULL, NULL);

  return CST_FLEX_ITEM_GET_IFACE(item)->get_padding(item);
}

const FrRect* cst_flex_item_get_bound (CstFlexItem *item) {
  sys_return_val_if_fail(item != NULL, NULL);

  return CST_FLEX_ITEM_GET_IFACE(item)->get_bound(item);
}
