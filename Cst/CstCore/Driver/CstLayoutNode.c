#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstLayout.h>

SYS_DEFINE_TYPE(CstLayoutNode, cst_layout_node, SYS_TYPE_OBJECT);

CstLayoutNode* cst_layout_node_new(void) {
  return sys_object_new(CST_TYPE_LAYOUT_NODE, NULL);
}

void cst_layout_node_set_size(CstLayoutNode* self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  self->bound.width = width;
  self->bound.height = height;
}

void cst_layout_node_get_size(CstLayoutNode* self, SysInt* width, SysInt* height) {
  sys_return_if_fail(self != NULL);

  *width = self->bound.width;
  *height = self->bound.height;
}

SysInt cst_layout_node_get_width(CstLayoutNode* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->bound.width;
}

void cst_layout_node_set_width(CstLayoutNode* self, SysInt width) {
  sys_return_if_fail(self != NULL);

  self->bound.width = width;
}

SysInt cst_layout_node_get_height(CstLayoutNode* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->bound.height;
}

void cst_layout_node_set_height(CstLayoutNode* self, SysInt height) {
  sys_return_if_fail(self != NULL);

  self->bound.height = height;
}

SysInt cst_layout_node_get_y(CstLayoutNode* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->bound.y;
}

void cst_layout_node_set_y(CstLayoutNode* self, SysInt y) {
  sys_return_if_fail(self != NULL);

  self->bound.y = y;
}

SysInt cst_layout_node_get_x(CstLayoutNode* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->bound.x;
}

void cst_layout_node_set_x(CstLayoutNode* self, SysInt x) {
  sys_return_if_fail(self != NULL);

  self->bound.x = x;
}

void cst_layout_node_set_xy(CstLayoutNode* self, SysInt x, SysInt y) {
  sys_return_if_fail(self != NULL);

  self->bound.x = x;
  self->bound.y = y;
}

void cst_layout_node_set_bound(CstLayoutNode* self, const FRRect *bound) {
  sys_return_if_fail(self != NULL);

  *(&self->bound) = *bound;
}

void cst_layout_node_get_mbp(CstLayoutNode* self, FRSInt4* m4) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(m4 != NULL);

  m4->m0 = self->margin.m0 + self->border.m0 + self->padding.m0;
  m4->m1 = self->margin.m1 + self->border.m1 + self->padding.m1;
  m4->m2 = self->margin.m2 + self->border.m2 + self->padding.m2;
  m4->m3 = self->margin.m3 + self->border.m3 + self->padding.m3;
}

CstLayoutNode* cst_layout_node_clone(CstLayoutNode* oself) {
  sys_return_val_if_fail(oself != NULL, NULL);

  CstLayoutNodeClass *cls = CST_LAYOUT_NODE_GET_CLASS(oself);
  sys_return_val_if_fail(cls->dclone != NULL, NULL);

  return cls->dclone(oself);
}

CstLayoutNode* cst_layout_node_clone_i(CstLayoutNode* oself) {
  SysType type = sys_type_from_instance(oself);
  CstLayoutNode *nself = sys_object_new(type, NULL);

  nself->bound.x = oself->bound.x;
  nself->bound.y = oself->bound.y;
  nself->bound.width = oself->bound.width;
  nself->bound.height = oself->bound.height;

  nself->margin = oself->margin;
  nself->border = oself->border;
  nself->padding = oself->padding;

  return nself;
}

void cst_layout_node_set_margin(CstLayoutNode *self, const FRSInt4 * margin) {
  sys_return_if_fail(self != NULL);

  self->margin = *margin;
}

const FRSInt4 * cst_layout_node_get_margin(CstLayoutNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return &self->margin;
}

void cst_layout_node_set_padding(CstLayoutNode *self, const FRSInt4 * padding) {
  sys_return_if_fail(self != NULL);

  self->padding = *padding;
}

const FRSInt4 * cst_layout_node_get_padding(CstLayoutNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return &self->padding;
}

void cst_layout_node_set_border(CstLayoutNode *self, const FRSInt4 * border) {
  sys_return_if_fail(self != NULL);

  self->border = *border;
}

const FRSInt4 * cst_layout_node_get_border(CstLayoutNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return &self->border;
}

void cst_layout_node_maybe_expand(CstLayoutNode* self, CstRenderContext *ctx) {
  sys_return_if_fail(self != NULL);
  SysInt pw, ph;

  cst_render_context_get_prefer_size(ctx, &pw, &ph);

  if (self->bound.width == -1) {
    self->bound.width = pw;
  }

  if (self->bound.height == -1) {
    self->bound.height = ph;
  }
}

const FRRect *cst_layout_node_get_bound(CstLayoutNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return &self->bound;
}

/* object api */
static void cst_layout_node_construct_i(CstLayoutNode *self, SysInt x, SysInt y, SysInt width, SysInt height) {
  self->bound.x = x;
  self->bound.y = y;
  self->bound.width = width;
  self->bound.height = height;
}

static void cst_layout_node_construct2_i(CstLayoutNode* self) {
  cst_layout_node_construct_i(self, 0, 0, -1, -1);
}

CstLayoutNode *cst_layout_node_new_I(SysInt x, SysInt y, SysInt width, SysInt height) {
  CstLayoutNode * o = cst_layout_node_new();

  cst_layout_node_construct_i(o, x, y, width, height);

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

  cls->construct = cst_layout_node_construct2_i;
  cls->dclone = cst_layout_node_clone_i;
}
