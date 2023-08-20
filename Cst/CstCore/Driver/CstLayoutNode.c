#include <CstCore/Driver/CstLayoutNode.h>


struct _CstLayoutNodePrivate {
  SysInt x;
  SysInt y;
  SysInt width;
  SysInt height;

  SysBool need_relayout;
  SysBool need_repaint;
  SysBool is_visible;
  SysBool wrap;
  FRRect bound;
  SysInt16 line_space;
  FRSInt4 border;
  FRSInt4 margin;
  FRSInt4 padding;

  SysInt child_count;
  FRSInt4 mbp;

  SysInt prefer_height;
  SysInt prefer_width;

  // self constraint
  CstCssClosure  *width_calc;
  CstCssClosure  *height_calc;

  // constraint for child, may be NULL.
  CstCssClosure  *child_width_calc;
  CstCssClosure  *child_height_calc;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstLayoutNode, cst_layout_node, SYS_TYPE_OBJECT);

CstLayoutNode* cst_layout_node_new(void) {
  return sys_object_new(CST_TYPE_LAYOUT_NODE, NULL);
}

void cst_layout_node_set_size(CstLayoutNode* self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  CstLayoutNodePrivate *priv = self->priv;

  priv->width = width;
  priv->height = height;
}

void cst_layout_node_get_size(CstLayoutNode* self, SysInt* width, SysInt* height) {
  sys_return_if_fail(self != NULL);

  CstLayoutNodePrivate *priv = self->priv;

  *width = priv->width;
  *height = priv->height;
}

SysInt cst_layout_node_get_width(CstLayoutNode* self) {
  sys_return_val_if_fail(self != NULL, -1);

  CstLayoutNodePrivate *priv = self->priv;

  return priv->width;
}

void cst_layout_node_set_width(CstLayoutNode* self, SysInt width) {
  sys_return_if_fail(self != NULL);

  CstLayoutNodePrivate *priv = self->priv;

  priv->width = width;
}

SysInt cst_layout_node_get_height(CstLayoutNode* self) {
  sys_return_val_if_fail(self != NULL, -1);

  CstLayoutNodePrivate *priv = self->priv;

  return priv->height;
}

void cst_layout_node_set_height(CstLayoutNode* self, SysInt height) {
  sys_return_if_fail(self != NULL);

  CstLayoutNodePrivate *priv = self->priv;

  priv->height = height;
}

SysInt cst_layout_node_get_y(CstLayoutNode* self) {
  sys_return_val_if_fail(self != NULL, -1);

  CstLayoutNodePrivate *priv = self->priv;

  return priv->y;
}

void cst_layout_node_set_y(CstLayoutNode* self, SysInt y) {
  sys_return_if_fail(self != NULL);

  CstLayoutNodePrivate *priv = self->priv;

  priv->y = y;
}

SysInt cst_layout_node_get_x(CstLayoutNode* self) {
  sys_return_val_if_fail(self != NULL, -1);

  CstLayoutNodePrivate *priv = self->priv;

  return priv->x;
}

void cst_layout_node_set_x(CstLayoutNode* self, SysInt x) {
  sys_return_if_fail(self != NULL);

  CstLayoutNodePrivate *priv = self->priv;

  priv->x = x;
}

void cst_layout_node_set_xy(CstLayoutNode* self, SysInt x, SysInt y) {
  sys_return_if_fail(self != NULL);

  CstLayoutNodePrivate *priv = self->priv;

  priv->x = x;
  priv->y = y;
}

void cst_layout_node_construct(CstLayoutNode *self, SysInt x, SysInt y, SysInt width, SysInt height) {
  CstLayoutNodePrivate *priv = self->priv;

  priv->x = x;
  priv->y = y;
  priv->width = width;
  priv->height = height;
}

CstLayoutNode *cst_layout_node_new_I(SysInt x, SysInt y, SysInt width, SysInt height) {
  CstLayoutNode * o = cst_layout_node_new();

  cst_layout_node_construct(o, x, y, width, height);

  return o;
}

CstLayoutNode* cst_layout_node_clone(CstLayoutNode* o) {
  sys_return_val_if_fail(o != NULL, NULL);

  SysType type = sys_type_from_instance(o);
  CstLayoutNode *n = sys_object_new(type, NULL);

  CstLayoutNodePrivate *opriv = o->priv;
  CstLayoutNodePrivate *npriv = n->priv;

  npriv->x = opriv->x;
  npriv->y = opriv->y;
  npriv->width = opriv->width;
  npriv->height = opriv->height;

  return n;
}

/* object api */
static void cst_layout_node_init(CstLayoutNode *self) {
  self->priv = cst_layout_node_get_private(self);
}

static void cst_layout_node_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  SYS_OBJECT_CLASS(cst_layout_node_parent_class)->dispose(o);
}

static void cst_layout_node_class_init(CstLayoutNodeClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layout_node_dispose;
}
