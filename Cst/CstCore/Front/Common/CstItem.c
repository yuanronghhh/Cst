#include <CstCore/Front/Common/CstItem.h>


struct _CstItemPrivate {
  SysChar reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstItem, cst_item, SYS_TYPE_OBJECT);

CstItem* cst_item_new(void) {
  return sys_object_new(CST_TYPE_ITEM, NULL);
}

void cst_item_set_size(CstItem* self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  self->width = width;
  self->height = height;
}

void cst_item_get_size(CstItem* self, SysInt* width, SysInt* height) {
  sys_return_if_fail(self != NULL);

  *width = self->width;
  *height = self->height;
}

SysInt cst_item_get_width(CstItem* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->width;
}

void cst_item_set_width(CstItem* self, SysInt width) {
  sys_return_if_fail(self != NULL);

  self->width = width;
}

SysInt cst_item_get_height(CstItem* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->height;
}

void cst_item_set_height(CstItem* self, SysInt height) {
  sys_return_if_fail(self != NULL);

  self->height = height;
}

SysInt cst_item_get_y(CstItem* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->y;
}

void cst_item_set_y(CstItem* self, SysInt y) {
  sys_return_if_fail(self != NULL);

  self->y = y;
}

SysInt cst_item_get_x(CstItem* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->x;
}

void cst_item_set_x(CstItem* self, SysInt x) {
  sys_return_if_fail(self != NULL);

  self->x = x;
}

void cst_item_set_xy(CstItem* self, SysInt x, SysInt y) {
  sys_return_if_fail(self != NULL);

  self->x = x;
  self->y = y;
}

void cst_item_construct(CstItem *self, SysInt x, SysInt y, SysInt width, SysInt height) {

  self->x = x;
  self->y = y;
  self->width = width;
  self->height = height;
}

CstItem *cst_item_new_I(SysInt x, SysInt y, SysInt width, SysInt height) {
  CstItem * o = cst_item_new();

  cst_item_construct(o, x, y, width, height);

  return o;
}

CstItem* cst_item_clone(CstItem* oitem) {
  sys_return_val_if_fail(oitem != NULL, NULL);

  SysType type = sys_type_from_instance(oitem);
  CstItem *n = sys_object_new(type, NULL);

  n->x = oitem->x;
  n->y = oitem->y;
  n->width = oitem->width;
  n->height = oitem->height;

  return n;
}

/* object api */
static void cst_item_init(CstItem *self) {
  self->priv = cst_item_get_private(self);
}

static void cst_item_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  SYS_OBJECT_CLASS(cst_item_parent_class)->dispose(o);
}

static void cst_item_class_init(CstItemClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_item_dispose;
}
