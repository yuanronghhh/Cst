#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstLayoutContext.h>
#include <CstCore/Driver/CstLayout.h>

SYS_DEFINE_TYPE(CstLayoutNode, cst_layout_node, SYS_TYPE_OBJECT);

CstLayoutNode* cst_layout_node_new(void) {
  return sys_object_new(CST_TYPE_LAYOUT_NODE, NULL);
}

void cst_layout_node_set_size(CstLayoutNode* self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  self->width = width;
  self->height = height;
}

void cst_layout_node_get_size(CstLayoutNode* self, SysInt* width, SysInt* height) {
  sys_return_if_fail(self != NULL);

  *width = self->width;
  *height = self->height;
}

SysInt cst_layout_node_get_width(CstLayoutNode* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->width;
}

void cst_layout_node_set_width(CstLayoutNode* self, SysInt width) {
  sys_return_if_fail(self != NULL);

  self->width = width;
}

SysInt cst_layout_node_get_height(CstLayoutNode* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->height;
}

void cst_layout_node_set_height(CstLayoutNode* self, SysInt height) {
  sys_return_if_fail(self != NULL);

  self->height = height;
}

SysInt cst_layout_node_get_y(CstLayoutNode* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->y;
}

void cst_layout_node_set_y(CstLayoutNode* self, SysInt y) {
  sys_return_if_fail(self != NULL);

  self->y = y;
}

SysInt cst_layout_node_get_x(CstLayoutNode* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->x;
}

void cst_layout_node_set_x(CstLayoutNode* self, SysInt x) {
  sys_return_if_fail(self != NULL);

  self->x = x;
}

void cst_layout_node_set_xy(CstLayoutNode* self, SysInt x, SysInt y) {
  sys_return_if_fail(self != NULL);

  self->x = x;
  self->y = y;
}

void cst_layout_node_set_bound(CstLayoutNode* self, SysInt x, SysInt y, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  self->x = x;
  self->y = y;
  self->width = width;
  self->height = height;
}

CstLayoutNode* cst_layout_node_clone(CstLayoutNode* oself) {
  sys_return_val_if_fail(oself != NULL, NULL);

  SysType type = sys_type_from_instance(oself);
  CstLayoutNode *nself = sys_object_new(type, NULL);

  nself->x = oself->x;
  nself->y = oself->y;
  nself->width = oself->width;
  nself->height = oself->height;

  return nself;
}

void cst_layout_node_set_padding(CstLayoutNode *self, FRSInt4 *m4) {
  sys_return_if_fail(self != NULL);

  self->padding = *m4;
}

void cst_layout_node_get_padding(CstLayoutNode *self, FRSInt4 *m4) {
  sys_return_if_fail(self != NULL);

  *m4 = self->padding;
}

void cst_layout_node_set_margin(CstLayoutNode *self, FRSInt4 *m4) {
  sys_return_if_fail(self != NULL);

  self->margin = *m4;
}

void cst_layout_node_get_margin(CstLayoutNode *self, FRSInt4 *m4) {
  sys_return_if_fail(self != NULL);

  *m4 = self->margin;
}

void cst_layout_node_set_border(CstLayoutNode* self, FRSInt4* m4) {
  sys_return_if_fail(self != NULL);

  self->border = *m4;
}

void cst_layout_node_get_border(CstLayoutNode* self, FRSInt4* m4) {
  sys_return_if_fail(self != NULL);

  *m4 = self->border;
}

void cst_layout_node_get_mbp(CstLayoutNode* self, FRSInt4* m4) {
  sys_return_if_fail(self != NULL);

  m4->m0 = self->margin.m0 + self->border.m0 + self->padding.m0;
  m4->m1 = self->margin.m1 + self->border.m1 + self->padding.m1;
  m4->m2 = self->margin.m2 + self->border.m2 + self->padding.m2;
  m4->m3 = self->margin.m3 + self->border.m3 + self->padding.m3;
}

void cst_layout_node_maybe_expand(CstLayoutNode* self, CstLayoutContext *ctx) {
  sys_return_if_fail(self != NULL);
  SysInt pw, ph;

  cst_layout_context_get_prefer_size(ctx, &pw, &ph);

  if (self->width == -1) {
    self->width = pw;
  }

  if (self->height == -1) {
    self->height = ph;
  }
}

/* object api */
static void cst_layout_node_construct(CstLayoutNode *self, SysInt x, SysInt y, SysInt width, SysInt height) {
  self->x = x;
  self->y = y;
  self->width = width;
  self->height = height;
}

CstLayoutNode *cst_layout_node_new_I(SysInt x, SysInt y, SysInt width, SysInt height) {
  CstLayoutNode * o = cst_layout_node_new();

  cst_layout_node_construct(o, x, y, width, height);

  return o;
}

static void cst_layout_node_init(CstLayoutNode *self) {
}

static void cst_layout_node_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  SYS_OBJECT_CLASS(cst_layout_node_parent_class)->dispose(o);
}

static void cst_layout_node_class_init(CstLayoutNodeClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layout_node_dispose;
}
