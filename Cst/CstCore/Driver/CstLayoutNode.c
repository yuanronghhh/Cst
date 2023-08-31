#include <CstCore/Driver/CstLayoutNode.h>
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

void cst_layout_node_construct(CstLayoutNode *self, SysInt x, SysInt y, SysInt width, SysInt height) {

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

void cst_layout_node_relayout(CstModule *v_module, CstLayoutNode *v_parent, CstLayoutNode *self, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstLayoutNodeClass* ncls = CST_LAYOUT_NODE_GET_CLASS(self);

  sys_return_if_fail(ncls->relayout != NULL);
  ncls->relayout(v_module, v_parent, self, draw, layout);
}

void cst_layout_node_relayout_down(CstModule *v_module, CstLayoutNode *v_parent, CstLayoutNode *self, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstLayoutNodeClass* ncls = CST_LAYOUT_NODE_GET_CLASS(self);

  sys_return_if_fail(ncls->relayout_down != NULL);
  ncls->relayout_down(v_module, v_parent, self, draw, layout);
}

static void cst_layout_node_relayout_i(CstModule *v_module, CstLayoutNode *v_parent, CstLayoutNode *self, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  if (v_parent) {
    self->child_count += 1;
  }
}

static void cst_layout_node_relayout_down_i(CstModule *v_module, CstLayoutNode *v_parent, CstLayoutNode *self, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  cst_layout_layout_children(layout, v_parent, self, draw);
}

/* object api */
static void cst_layout_node_init(CstLayoutNode *self) {
}

static void cst_layout_node_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  SYS_OBJECT_CLASS(cst_layout_node_parent_class)->dispose(o);
}

static void cst_layout_node_class_init(CstLayoutNodeClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layout_node_dispose;

  cls->relayout = cst_layout_node_relayout_i;
  cls->relayout_down = cst_layout_node_relayout_down_i;
}
