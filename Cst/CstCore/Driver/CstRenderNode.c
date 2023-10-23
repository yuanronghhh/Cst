#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstLayout.h>


SYS_DEFINE_TYPE(CstRenderNode, cst_render_node, SYS_TYPE_OBJECT);


static const SysChar* CST_RENDER_NODE_NAMES[] = {
  "box", "absolute"
};

CST_RENDER_NODE_ENUM cst_render_node_type_by_name(const SysChar* name) {
  return fr_get_type_by_name(CST_RENDER_NODE_NAMES, ARRAY_SIZE(CST_RENDER_NODE_NAMES), name);
}

void cst_render_node_prepare(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  FRSInt4 m4 = { 0 };
  CstLayoutNode *lnode = cst_render_node_get_lnode(self);

  cst_layout_node_get_mbp(lnode, &m4);
  cst_render_context_set_mbp(self->render_ctx, &m4);
}

CstRenderNode* cst_render_node_get_parent(CstRenderNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstRenderNodeClass *cls = CST_RENDER_NODE_GET_CLASS(self);
  sys_return_val_if_fail(cls->get_parent != NULL, NULL);

  return cls->get_parent(self);
}

void cst_render_node_set_render_ctx(CstRenderNode *self, CstRenderContext * render_ctx) {
  sys_return_if_fail(self != NULL);

  self->render_ctx = render_ctx;
}

CstRenderContext * cst_render_node_get_render_ctx(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->render_ctx;
}

CstLayoutNode* cst_render_node_get_lnode(CstRenderNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return CST_LAYOUT_NODE(self->node);
}

CstNode * cst_render_node_get_node(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->node;
}

void cst_render_node_render_enter(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
  FRDraw* draw = cst_layout_get_draw(layout);

  fr_draw_save(draw);
  cst_render_node_prepare(self, layout);
  cst_node_render_css(CST_RENDER_NODE_NODE(self), self, layout);
  cst_render_context_calc_size(self->render_ctx, layout, self);
}

void cst_render_node_render_leave(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
  FRDraw* draw = cst_layout_get_draw(layout);

  fr_draw_restore(draw);
}

SysObject* cst_render_node_dclone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  SysObject *n = SYS_OBJECT_CLASS(cst_render_node_parent_class)->dclone(o);

  CstRenderNode *nself = CST_RENDER_NODE(n);
  CstRenderNode *oself = CST_RENDER_NODE(o);

  nself->node = (CstNode *)sys_object_dclone(oself->node);
  nself->render_ctx = (CstRenderContext *)sys_object_dclone(oself->render_ctx);

  return n;
}

void cst_render_node_print(CstRenderNode* self) {
  sys_return_if_fail(self != NULL);

  CstNode* node;
  CstNode* pnode;
  CstRenderNode* prnode;
  CstLayoutNode* lnode;
  const FRRect* bound;

  node = CST_RENDER_NODE_NODE(self);
  lnode = CST_LAYOUT_NODE(node);
  bound = cst_layout_node_get_bound(lnode);
  prnode = cst_render_node_get_parent(self);

  if (prnode) {
    pnode = cst_render_node_get_node(prnode);

    sys_debug_N("%s,%s<%d,%d,%d,%d>",
      cst_node_get_name(pnode),
      cst_node_get_name(node),
      bound->x, bound->y, bound->width, bound->height);

  } else {

    sys_debug_N("%s<%d,%d,%d,%d>",
      cst_node_get_name(node),
      bound->x, bound->y, bound->width, bound->height);
  }
}

SysType cst_render_node_get_node_type(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, 0);
  sys_return_val_if_fail(self->node != NULL, 0);

  return sys_type_from_instance(CST_RENDER_NODE_NODE(self));
}

/* object api */
static void cst_render_node_dispose(SysObject* o) {
  CstRenderNode* self = CST_RENDER_NODE(o);

  sys_clear_pointer(&self->node, _sys_object_unref);
  sys_clear_pointer(&self->render_ctx, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_render_node_parent_class)->dispose(o);
}

static void cst_render_node_construct(CstRenderNode* self, CstNode *node) {
  self->node = (CstNode *)sys_object_dclone(node);
  self->render_ctx = cst_node_create_default_context(node);

  sys_object_ref(node);
}

CstRenderNode *cst_render_node_new(void) {
  return sys_object_new(CST_TYPE_RENDER_NODE, NULL);
}

CstRenderNode *cst_render_node_new_I(CstNode *node) {
  CstRenderNode *o = cst_render_node_new();

  cst_render_node_construct(CST_RENDER_NODE(o), node);

  return o;
}

static void cst_render_node_class_init(CstRenderNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_render_node_dispose;
  cls->construct = cst_render_node_construct;
}

static void cst_render_node_init(CstRenderNode *self) {
}

