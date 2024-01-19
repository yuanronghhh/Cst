#include <CstCore/Driver/CstFlexItem.h>


SYS_DEFINE_INTERFACE(CstFlexItem, cst_flex_item, SYS_TYPE_OBJECT);

/* object api */
void cst_flex_item_default_init(CstFlexItemInterface* iface) {
}

SysInt cst_flex_item_get_width (CstFlexItem *item) {
  sys_return_val_if_fail(item != NULL, -1);

  return CST_FLEX_ITEM_GET_IFACE(item)->get_width(item);
}
