#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/Css/CstCssClosure.h>
#include <CstCore/Driver/CstRenderNode.h>

SYS_DEFINE_TYPE(CstRenderContext, cst_render_context, SYS_TYPE_OBJECT);


CstRenderContext* cst_render_context_new(void) {
  return sys_object_new(CST_TYPE_LAYOUT_CONTEXT, NULL);
}

void cst_render_context_set_mbp(CstRenderContext* self, FRSInt4* m4) {
  sys_return_if_fail(self != NULL);

  self->mbp.m0 = m4->m0;
  self->mbp.m1 = m4->m1;
  self->mbp.m2 = m4->m2;
  self->mbp.m3 = m4->m3;
}

const FRSInt4* cst_render_context_get_mbp(CstRenderContext* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return &self->mbp;
}

SysBool cst_render_context_is_abs_node(CstRenderContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->node_type & CST_RENDER_NODE_ABS;
}

void cst_render_context_set_abs_node(CstRenderContext* self, SysInt bvalue) {
  sys_return_if_fail(self != NULL);

  self->node_type &= bvalue;
}

SysBool cst_render_context_is_box_node(CstRenderContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->node_type & CST_RENDER_NODE_BOX;
}

void cst_render_context_set_node_type(CstRenderContext *self, SysInt flag) {
  sys_return_if_fail(self != NULL);

  self->node_type = flag;
}

void cst_render_context_set_width_closure(CstRenderContext* self, CstCssClosure* c) {
  sys_return_if_fail(self != NULL);

  if (self->width_calc) {
    sys_clear_pointer(&self->width_calc, _sys_object_unref);
  }

  self->width_calc = c;
}

void cst_render_context_set_height_closure(CstRenderContext* self, CstCssClosure* c) {
  sys_return_if_fail(self != NULL);

  if (self->height_calc) {
    sys_clear_pointer(&self->height_calc, _sys_object_unref);
  }

  self->height_calc = c;
}

void cst_render_context_set_prefer_size(CstRenderContext* self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  self->prefer_width = width;
  self->prefer_height = height;
}

void cst_render_context_get_prefer_size(CstRenderContext* self, SysInt* width, SysInt* height) {
  sys_return_if_fail(self != NULL);

  *width = self->prefer_width;
  *height = self->prefer_height;
}

SysBool cst_render_context_need_layout(CstRenderContext *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->need_relayout;
}

void cst_render_context_set_layout(CstRenderContext *self, SysBool bvalue) {
  sys_return_if_fail(self != NULL);

  self->need_relayout = bvalue;
}

void cst_render_context_calc_size(CstRenderContext *self, CstRenderNode *rnode) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(rnode != NULL);

  cst_css_closure_calc(self->width_calc, rnode);
  cst_css_closure_calc(self->height_calc, rnode);
}

SysBool cst_render_context_can_wrap(CstRenderContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->wrap;
}

void cst_render_context_layout_self(CstRenderContext* self, CstRenderNode *rnode, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

  CstRenderContextClass *cls = CST_RENDER_CONTEXT_GET_CLASS(self);
  sys_return_if_fail(cls->layout_self != NULL);

  cls->layout_self(self, rnode, layout);
}

void cst_render_context_layout_children(CstRenderContext* self, CstRenderNode *rnode, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

  CstRenderContextClass *cls = CST_RENDER_CONTEXT_GET_CLASS(self);
  sys_return_if_fail(cls->layout_children != NULL);

  cls->layout_children(self, rnode, layout);
}

CstRenderContext* cst_render_context_dclone(CstRenderContext *o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysType type = sys_type_from_instance(o);
  CstRenderContext *n = sys_object_new(type, NULL);

  n->need_relayout = o->need_relayout;
  n->need_repaint = o->need_repaint;
  n->is_visible = o->is_visible;
  n->wrap = o->wrap;
  n->line_space = o->line_space;
  n->node_type = o->node_type;
  n->child_count = o->child_count;
  n->mbp = o->mbp;
  n->prefer_height = o->prefer_height;
  n->prefer_width = o->prefer_width;

  return n;
}

void cst_render_context_layout_self_i(CstRenderContext* self, CstRenderNode *rnode, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

  // SysInt w = 0, h = 0;
  // FRSInt4 *mbp = &self->mbp;
  // 
  // cst_layout_node_get_size(layout_node, &w, &h);
  // 
  // w = w + mbp->m1 + mbp->m3;
  // h = h + mbp->m2 + mbp->m0;
  // 
  // if (pself->child_width_calc) {
  //   pself->prefer_width = max(w, pself->prefer_width);
  //   cst_css_closure_calc(pself->child_width_calc, layout_node, self, NULL, NULL);
  // }
  // 
  // if (pself->child_height_calc) {
  //   pself->prefer_height = max(h, pself->prefer_height);
  //   cst_css_closure_calc(pself->child_height_calc, layout_node, self, NULL, NULL);
  // }
  // 
  // pself->prefer_width += w;
  // pself->prefer_height = max(h, pself->prefer_height);
}

/* constraint */
void cst_render_context_constrain_same_width(CstRenderNode* self, CstRenderContext* pctx) {
  CstRenderContext* rctx = cst_render_node_get_context(self);
  CstLayoutNode* layout_node = CST_LAYOUT_NODE(self);

  layout_node->bound.height = pctx->prefer_width - rctx->mbp.m3 - rctx->mbp.m1;
}

void cst_render_context_constrain_same_height(CstRenderNode* self, CstRenderContext* pctx) {
  CstRenderContext* rctx = cst_render_node_get_context(self);
  CstLayoutNode* layout_node = CST_LAYOUT_NODE(self);

  layout_node->bound.height = pctx->prefer_height - rctx->mbp.m0 - rctx->mbp.m2;
}

/* paint */
SysBool cst_render_context_need_paint(CstRenderContext *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->need_repaint;
}

void cst_render_context_set_paint(CstRenderContext *self, SysBool bvalue) {
  sys_return_if_fail(self != NULL);

  self->need_repaint = bvalue;
}

SysBool cst_render_context_is_visible(CstRenderContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->is_visible;
}

/* object api */
CstRenderContext *cst_render_context_new_I(void) {
  CstRenderContext *o = cst_render_context_new();
  return o;
}

static void cst_render_context_init(CstRenderContext *self) {
}

static void cst_render_context_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  SYS_OBJECT_CLASS(cst_render_context_parent_class)->dispose(o);
}

static void cst_render_context_class_init(CstRenderContextClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_render_context_dispose;
}
