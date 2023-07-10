#ifndef __CST_ITEM__
#define __CST_ITEM__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_ITEM (cst_item_get_type())
#define CST_ITEM(o) ((CstItem* )sys_object_cast_check(o, CST_TYPE_ITEM))
#define CST_ITEM_CLASS(o) ((CstItemClass *)sys_class_cast_check(o, CST_TYPE_ITEM))
#define CST_ITEM_GET_CLASS(o) sys_instance_get_class(o, CstItemClass)

struct _CstItem {
  SysObject parent;

  SysInt x;
  SysInt y;
  SysInt width;
  SysInt height;

  CstItemPrivate *priv;
};

struct _CstItemClass {
  SysObjectClass parent;
};

SysType cst_item_get_type(void);
CstItem *cst_item_new(void);
CstItem *cst_item_new_I(SysInt x, SysInt y, SysInt width, SysInt height);
CstItem* cst_item_clone(CstItem* oitem);

void cst_item_set_size(CstItem* self, SysInt width, SysInt height);
void cst_item_get_size(CstItem* self, SysInt *width, SysInt *height);

void cst_item_set_width(CstItem* self, SysInt width);
SysInt cst_item_get_width(CstItem* self);
void cst_item_set_height(CstItem* self, SysInt width);
SysInt cst_item_get_height(CstItem* self);

void cst_item_set_x(CstItem* self, SysInt x);
void cst_item_set_xy(CstItem* self, SysInt x, SysInt y);
SysInt cst_item_get_x(CstItem* self);
void cst_item_set_y(CstItem* self, SysInt width);
SysInt cst_item_get_y(CstItem* self);

SYS_END_DECLS

#endif

