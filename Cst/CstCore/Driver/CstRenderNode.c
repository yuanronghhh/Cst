#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstComponent.h>


SYS_DEFINE_TYPE(CstRenderNode, cst_render_node, SYS_TYPE_OBJECT);


static const SysChar* CST_RENDER_NODE_NAMES[] = {
  "box", "absolute"
};

CST_RENDER_NODE_ENUM cst_render_node_type_by_name(const SysChar* name) {
  return fr_get_type_by_name(CST_RENDER_NODE_NAMES, ARRAY_SIZE(CST_RENDER_NODE_NAMES), name);
}

void cst_render_node_set_layout(CstRenderNode *self, SysBool bvalue) {
  sys_return_if_fail(self != NULL);

  cst_render_context_set_layout(self->render_ctx, bvalue);
}

SysBool cst_render_node_need_layout(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_render_context_need_layout(self->render_ctx);
}

void cst_render_node_set_paint(CstRenderNode *self, SysBool bvalue) {
  sys_return_if_fail(self != NULL);

  cst_render_context_set_paint(self->render_ctx, bvalue);
}

SysBool cst_render_node_need_paint(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_render_context_need_paint(self->render_ctx);
}

void cst_render_node_set_wrap(CstRenderNode *self, SysBool wrap) {
  sys_return_if_fail(self != NULL);

  cst_render_context_set_wrap(self->render_ctx, wrap);
}

SysBool cst_render_node_get_wrap(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_render_context_get_wrap(self->render_ctx);
}

const FRRect *cst_render_node_get_bound(CstRenderNode* self) {

  return cst_layout_node_get_bound(CST_LAYOUT_NODE(self->node));
}

void cst_render_node_get_mbp(CstRenderNode* self, FRSInt4* m4) {

  cst_layout_node_get_mbp(CST_LAYOUT_NODE(self->node), m4);
}

void cst_render_node_set_x(CstRenderNode* self, SysInt x) {

  cst_layout_node_set_x(CST_LAYOUT_NODE(self->node), x);
}

SysInt cst_render_node_get_x(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return cst_layout_node_get_x(CST_LAYOUT_NODE(self->node));
}

void cst_render_node_set_y(CstRenderNode *self, SysInt y) {
  sys_return_if_fail(self != NULL);

  cst_layout_node_set_y(CST_LAYOUT_NODE(self->node), y);
}

SysInt cst_render_node_get_y(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return cst_layout_node_get_y(CST_LAYOUT_NODE(self->node));
}

void cst_render_node_set_width(CstRenderNode* self, SysInt width) {
  sys_return_if_fail(self != NULL);

  cst_layout_node_set_width(CST_LAYOUT_NODE(self->node), width);
}

void cst_render_node_set_size(CstRenderNode* self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  cst_layout_node_set_size(CST_LAYOUT_NODE(self->node), width, height);
}

void cst_render_node_set_margin(CstRenderNode *self, const FRSInt4 * margin) {
  sys_return_if_fail(self != NULL);

  cst_layout_node_set_margin(CST_LAYOUT_NODE(self->node), margin);
}

const FRSInt4 * cst_render_node_get_margin(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return cst_layout_node_get_margin(CST_LAYOUT_NODE(self->node));
}

void cst_render_node_set_border(CstRenderNode *self, const FRSInt4 * border) {
  sys_return_if_fail(self != NULL);

  cst_layout_node_set_border(CST_LAYOUT_NODE(self->node), border);
}

const FRSInt4 * cst_render_node_get_border(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return cst_layout_node_get_border(CST_LAYOUT_NODE(self->node));
}

void cst_render_node_set_padding(CstRenderNode *self, const FRSInt4 * padding) {
  sys_return_if_fail(self != NULL);

  cst_layout_node_set_padding(CST_LAYOUT_NODE(self->node), padding);
}

const FRSInt4 * cst_render_node_get_padding(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return cst_layout_node_get_padding(CST_LAYOUT_NODE(self->node));
}

void cst_render_node_set_height(CstRenderNode* self, SysInt height) {
  sys_return_if_fail(self != NULL);

  cst_layout_node_set_height(CST_LAYOUT_NODE(self->node), height);
}

void cst_render_node_set_position(CstRenderNode *self, SysInt position) {
  sys_return_if_fail(self != NULL);

  cst_render_context_set_position(self->render_ctx, position);
}

SysInt cst_render_node_get_position(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return cst_render_context_get_position(self->render_ctx);
}

void cst_render_node_prepare(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  FRSInt4 m4 = { 0 };
  CstLayoutNode *lnode = CST_LAYOUT_NODE(self->node);

  cst_layout_node_get_mbp(lnode, &m4);
  cst_render_context_set_mbp(self->render_ctx, &m4);
  cst_render_context_calc_width(self->render_ctx, layout, self);
}

SysBool cst_render_node_is_dirty(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_render_context_need_layout(self->render_ctx)
    || cst_render_context_need_paint(self->render_ctx);
}

void cst_render_node_mark_dirty(CstRenderNode* self, SysBool bvalue) {
}

SysBool cst_render_node_is_visible(CstRenderNode* self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_render_context_is_visible(self->render_ctx);
}

SysBool cst_render_node_can_wrap(CstRenderNode* self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_render_context_get_wrap(self->render_ctx);
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

void cst_render_node_render_enter(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  cst_node_render_css(self->node, self, layout);
  cst_render_node_prepare(self, layout);
}

void cst_render_node_render_leave(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
}

CstRenderNode* cst_render_node_dclone_i(CstRenderNode *o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysType type = sys_type_from_instance(o);

  CstRenderNode *n = sys_object_new(type, NULL);

  n->node = cst_node_dclone(o->node);
  n->render_ctx = cst_render_context_dclone(o->render_ctx);

  return n;
}

void cst_render_node_relayout_self(CstRenderNode *self, CstLayout *layout) {

  cst_render_context_layout_self(self->render_ctx, self, layout);
}

const SysChar* cst_render_node_get_id(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return cst_node_get_id(self->node);
}

const SysChar* cst_render_node_get_name(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return cst_node_get_name(self->node);
}

void cst_render_node_set_name(CstRenderNode* self, const SysChar*name) {
  sys_return_if_fail(self != NULL);

  cst_node_set_name(self->node, name);
}

void cst_render_node_paint_self(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
}

void cst_render_node_fill_rectangle(CstRenderNode *self, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

  const FRRect* bound = cst_render_node_get_bound(self);
  FRDraw* draw = cst_layout_get_draw(layout);

  fr_draw_fill_rectangle(draw, bound);
}

void cst_render_node_stroke_rectangle(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  const FRSInt4 *m4, *p4;
  const FRRect* bound;
  FRDraw* draw = cst_layout_get_draw(layout);

  m4 = cst_layout_node_get_margin(CST_LAYOUT_NODE(self->node));
  p4 = cst_layout_node_get_padding(CST_LAYOUT_NODE(self->node));

  bound = cst_render_node_get_bound(self);

  fr_draw_stroke_mp(draw, bound, m4, p4);

#if 0
  sys_debug_N("repaint render_node: %s,%s<%d,%d,%d,%d>",
      cst_render_node_get_id(self),
      cst_render_node_get_name(self),
      bound->x,
      bound->y,
      bound->width,
      bound->height);
#endif
}

void cst_render_node_layout(CstRenderNode* self, CstLayout *layout) {

  cst_render_context_layout_self(self->render_ctx, self, layout);
}

void cst_render_node_constraint_size(CstRenderNode* self, CstRenderContext* pctx) {
  CstLayoutNode* layout_node = CST_LAYOUT_NODE(self->node);

  cst_render_context_constraint_width(self->render_ctx, pctx, &layout_node->bound.width);
  cst_render_context_constraint_height(self->render_ctx, pctx, &layout_node->bound.height);
}

void cst_render_node_constraint_width(CstRenderNode* self, CstRenderContext* pctx) {
  CstLayoutNode* layout_node = CST_LAYOUT_NODE(self->node);

  cst_render_context_constraint_width(self->render_ctx, pctx, &layout_node->bound.width);
}

void cst_render_node_constraint_height(CstRenderNode* self, CstRenderContext* pctx) {
  CstLayoutNode* layout_node = CST_LAYOUT_NODE(self->node);

  cst_render_context_constraint_height(self->render_ctx, pctx, &layout_node->bound.height);
}

SysInt cst_render_node_check_dirty(CstRenderNode* rnode, FRRegion* region) {
  const FRRect* nbound = cst_render_node_get_bound(rnode);

  if (fr_region_is_empty(region)) {
    return -4;
  }

  if (cst_render_node_is_dirty(rnode)) {
    return -1;
  }

  if (!cst_render_node_is_visible(rnode)) {
    return -1;
  }

  sys_assert(nbound->width != -1 && "width should be set before check dirty.");
  SysInt s = fr_region_contains_rectangle(region, nbound);
  if (s == CAIRO_REGION_OVERLAP_OUT) {
    return -3;
  }

  return 1;
}

void cst_render_node_print(CstRenderNode* self) {
  sys_return_if_fail(self != NULL);

  const FRRect *bound = cst_render_node_get_bound(self);
  sys_debug_N("%s,%s<%d,%d,%d,%d>",
    cst_render_node_get_name(self),
    cst_render_node_get_id(self),
    bound->x, bound->y, bound->width, bound->height);
}

/* object api */
static void cst_render_node_dispose(SysObject* o) {
  CstRenderNode *self = CST_RENDER_NODE(o);

  SYS_OBJECT_CLASS(cst_render_node_parent_class)->dispose(o);
}

static void cst_render_node_construct(CstRenderNode* self, CstNode *node) {
  CST_LAYOUT_NODE_CLASS(cst_render_node_parent_class)->construct(CST_LAYOUT_NODE(self->node));

  self->node = node;
  self->render_ctx = cst_node_new_default_context(node);

  sys_object_ref(SYS_OBJECT(node));
}

CstLayoutNode *cst_render_node_new(void) {
  return sys_object_new(CST_TYPE_RENDER_NODE, NULL);
}

CstLayoutNode *cst_render_node_new_I(CstNode *node) {
  CstLayoutNode *o = cst_render_node_new();

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
