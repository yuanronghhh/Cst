#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstLayoutContext.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Front/CstComponent.h>


SYS_DEFINE_TYPE(CstRenderNode, cst_render_node, SYS_TYPE_OBJECT);


static const SysChar* CST_RENDER_NODE_NAMES[] = {
  "box", "absolute"
};

CST_RENDER_NODE_ENUM cst_render_node_get_by_name(const SysChar* name) {
  return fr_get_type_by_name(CST_RENDER_NODE_NAMES, ARRAY_SIZE(CST_RENDER_NODE_NAMES), name);
}

void cst_render_node_set_layout(CstRenderNode *self, SysBool bvalue) {
  sys_return_if_fail(self != NULL);

  cst_layout_context_set_layout(self->layout_ctx, bvalue);
}

SysBool cst_render_node_need_layout(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_layout_context_need_layout(self->layout_ctx);
}

void cst_render_node_set_paint(CstRenderNode *self, SysBool bvalue) {
  sys_return_if_fail(self != NULL);

  cst_layout_context_set_paint(self->layout_ctx, bvalue);
}

SysBool cst_render_node_need_paint(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_layout_context_need_paint(self->layout_ctx);
}

void cst_render_node_set_abs_node(CstRenderNode *self, SysBool bvalue) {
  sys_return_if_fail(self != NULL);

  cst_layout_context_set_abs_node(self->layout_ctx, bvalue);
}

SysBool cst_render_node_is_abs_node(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_layout_context_is_abs_node(self->layout_ctx);
}

const FRRect *cst_render_node_get_bound(CstRenderNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return cst_layout_node_get_bound(self->layout_node);
}

void cst_render_node_set_x(CstRenderNode* self, SysInt x) {

  cst_layout_node_set_x(self->layout_node, x);
}

void cst_render_node_set_width(CstRenderNode* self, SysInt width) {
  sys_return_if_fail(self != NULL);

  cst_layout_node_set_width(self->layout_node, width);
}

void cst_render_node_set_height(CstRenderNode* self, SysInt height) {
  sys_return_if_fail(self != NULL);

  cst_layout_node_set_height(self->layout_node, height);
}

void cst_render_node_prepare(CstRenderNode *self) {
  sys_return_if_fail(self != NULL);

  FRSInt4 m4 = { 0 };

  cst_layout_node_get_mbp(self->layout_node, &m4);
  cst_layout_context_set_mbp(self->layout_ctx, &m4);
  cst_layout_context_calc_size(self->layout_ctx, self);
}

void cst_render_node_get_mbp(CstRenderNode* self, FRSInt4* m4) {
  sys_return_if_fail(self != NULL);

  cst_layout_node_get_mbp(self->layout_node, m4);
}

SysBool cst_render_node_is_dirty(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_layout_context_need_layout(self->layout_ctx)
    || cst_layout_context_need_paint(self->layout_ctx);
}

void cst_render_node_mark_dirty(CstRenderNode* self, SysBool bvalue) {
}

SysBool cst_render_node_is_visible(CstRenderNode* self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_layout_context_is_visible(self->layout_ctx);
}

SysBool cst_render_node_can_wrap(CstRenderNode* self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_layout_context_can_wrap(self->layout_ctx);
}

CstRenderNode* cst_render_node_get_parent(CstRenderNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstRenderNodeClass *cls = CST_RENDER_NODE_GET_CLASS(self);
  sys_return_val_if_fail(cls->get_parent == NULL, NULL);

  return cls->get_parent(self);
}

void cst_render_node_render_enter(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  // cst_css_render_groups(self->css_groups, self, layout);
  cst_render_node_prepare(self);
}

void cst_render_node_render_leave(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
}

CstRenderNode* cst_render_node_dclone_i(CstRenderNode *o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysType type = sys_type_from_instance(o);

  CstRenderNode *n = sys_object_new(type, NULL);

  n->node = cst_node_dclone(o->node);
  n->layout_node = cst_layout_node_clone(o->layout_node);
  n->layout_ctx = cst_layout_context_dclone(o->layout_ctx);

  return n;
}

void cst_render_node_relayout_self(CstRenderNode *self, CstLayout *layout) {

  cst_layout_context_layout_self(self->layout_ctx, self, layout);
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

  FRSInt4 m4, p4;
  const FRRect* bound;
  FRDraw* draw = cst_layout_get_draw(layout);

  cst_layout_node_get_margin(self->layout_node, &m4);
  cst_layout_node_get_padding(self->layout_node, &p4);

  bound = cst_render_node_get_bound(self);

  fr_draw_stroke_mp(draw, bound, &m4, &p4);

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

CstLayoutContext* cst_render_get_layout_context(CstRenderNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->layout_ctx;
}

CstLayoutNode* cst_render_get_layout_node(CstRenderNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->layout_node;
}

/* object api */
static void cst_render_node_dispose(SysObject* o) {
  CstRenderNode *self = CST_RENDER_NODE(o);

  SYS_OBJECT_CLASS(cst_render_node_parent_class)->dispose(o);
}

static void cst_render_node_construct(CstRenderNode* self, CstNode *node, CstLayoutContext *layout_ctx) {
  self->node = node;
  self->layout_node = cst_layout_node_new_I(0, 0, -1, 1);
  self->layout_ctx = layout_ctx;

  sys_object_ref(SYS_OBJECT(node));
}

CstRenderNode *cst_render_node_new(void) {
  return sys_object_new(CST_TYPE_RENDER_NODE, NULL);
}

CstRenderNode *cst_render_node_new_I(CstNode *node, CstLayoutContext* layout_ctx) {
  CstRenderNode *o = cst_render_node_new();

  cst_render_node_construct(o, node, layout_ctx);

  return o;
}

static void cst_render_node_class_init(CstRenderNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_render_node_dispose;

  cls->construct = cst_render_node_construct;
}

static void cst_render_node_init(CstRenderNode *self) {
}
