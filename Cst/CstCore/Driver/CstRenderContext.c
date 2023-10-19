#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/Css/CstCssClosure.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstBoxNode.h>


SYS_DEFINE_TYPE(CstRenderContext, cst_render_context, SYS_TYPE_OBJECT);

static SysHashTable* grctx_ht = NULL;

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

void cst_render_context_set_position(CstRenderContext *self, SysInt position) {
  sys_return_if_fail(self != NULL);

  self->position = position;
}

SysInt cst_render_context_get_position(CstRenderContext *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->position;
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

void cst_render_context_calc_width(CstRenderContext *self, CstLayout *layout, CstRenderNode *rnode) {

  cst_css_closure_calc(self->width_calc, layout, rnode);
}

void cst_render_context_calc_height(CstRenderContext *self, CstLayout *layout, CstRenderNode *rnode) {

  cst_css_closure_calc(self->height_calc, layout, rnode);
}

  void cst_render_context_calc_size(CstRenderContext *self, CstLayout *layout, CstRenderNode *rnode) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(rnode != NULL);

  cst_css_closure_calc(self->width_calc, layout, rnode);
  cst_css_closure_calc(self->height_calc, layout, rnode);
}

void cst_render_context_set_wrap(CstRenderContext *self, SysBool wrap) {
  sys_return_if_fail(self != NULL);

  self->wrap = wrap;
}

SysBool cst_render_context_get_wrap(CstRenderContext *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->wrap;
}

CstRenderContext* cst_render_context_dclone(CstRenderContext* o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysType type = sys_type_from_instance(o);
  CstRenderContext* n = sys_object_new(type, NULL);

  n->need_relayout = o->need_relayout;
  n->need_repaint = o->need_repaint;
  n->is_visible = o->is_visible;
  n->wrap = o->wrap;
  n->line_space = o->line_space;
  n->position = o->position;
  n->child_count = o->child_count;
  n->mbp = o->mbp;
  n->prefer_height = o->prefer_height;
  n->prefer_width = o->prefer_width;

  return n;
}

void cst_render_context_set_is_visible(CstRenderContext *self, SysBool is_visible) {
  sys_return_if_fail(self != NULL);

  self->is_visible = is_visible;
}

SysBool cst_render_context_get_is_visible(CstRenderContext *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->is_visible;
}

void cst_render_context_set_need_relayout(CstRenderContext *self, SysBool need_relayout) {
  sys_return_if_fail(self != NULL);

  self->need_relayout = need_relayout;
}

SysBool cst_render_context_get_need_relayout(CstRenderContext *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->need_relayout;
}

void cst_render_context_set_direction(CstRenderContext *self, SysInt direction) {
  sys_return_if_fail(self != NULL);

  self->direction = direction;
}

SysInt cst_render_context_get_direction(CstRenderContext *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->direction;
}

void cst_render_context_inherit(CstRenderContext *self, CstRenderContext *pctx, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  if (pctx) {

    self->prefer_width = pctx->prefer_width;
    self->prefer_height = pctx->prefer_height;
  }
}

void cst_render_context_layout_self(CstRenderContext *self, CstRenderNode *rnode, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstRenderContextClass* cls = CST_RENDER_CONTEXT_GET_CLASS(self);
  sys_return_if_fail(cls->layout_self != NULL);

  cls->layout_self(self, rnode, layout);
}

void cst_render_context_layout_children(CstRenderContext *self, CstRenderNode *rnode, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstRenderContextClass* cls = CST_RENDER_CONTEXT_GET_CLASS(self);
  sys_return_if_fail(cls->layout_children != NULL);

  cls->layout_children(self, rnode, layout);
}

void cst_render_context_layout_self_i(CstRenderContext *self, CstRenderNode *rnode, CstLayout *layout) {
  CstLayoutNode* lnode = cst_render_node_get_lnode(rnode);
  SysInt w, h;

  cst_layout_node_get_size(lnode, &w, &h);

  if (w == -1) {

    cst_layout_node_set_width(lnode, self->prefer_width);
  }

  if (h == -1) {

    cst_layout_node_set_height(lnode, self->prefer_height);
  }
}

void cst_render_context_layout_children_i(CstRenderContext *self, CstRenderNode *rnode, CstLayout *layout) {
}

/* constraint */
void cst_render_context_constraint_width(CstRenderContext* self, CstRenderContext* pctx, SysInt* width) {

  *width = pctx->prefer_width - self->mbp.m3 - self->mbp.m1;
  sys_assert(*width >= 0);
}

void cst_render_context_constraint_height(CstRenderContext* self, CstRenderContext* pctx, SysInt* height) {

  *height = pctx->prefer_height - self->mbp.m0 - self->mbp.m2;
  sys_assert(*height >= 0);
}

SysBool cst_render_context_is_dirty(CstRenderContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->need_relayout || self->need_repaint;
}

/* paint */
SysBool cst_render_context_need_paint(CstRenderContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->need_repaint;
}

void cst_render_context_set_paint(CstRenderContext* self, SysBool bvalue) {
  sys_return_if_fail(self != NULL);

  self->need_repaint = bvalue;
}

void cst_render_context_setup(void) {

  grctx_ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, NULL);
}

void cst_render_context_teardown(void) {
  sys_assert(grctx_ht != NULL && "render context should init before use.");

  sys_hash_table_unref(grctx_ht);
}

void constraint_width(CstRenderNode* rnode, CstLayout* layout, SysPointer user_data) {
}

void constraint_height(CstRenderNode* rnode, CstLayout* layout, SysPointer user_data) {
}

/* object api */
CstRenderContext *cst_render_context_new_I(void) {
  CstRenderContext *o = cst_render_context_new();

  return o;
}

static void cst_render_context_init(CstRenderContext *self) {
  self->width_calc = cst_css_closure_new_I(self, constraint_width, NULL);
  self->height_calc = cst_css_closure_new_I(self, constraint_height, NULL);
  self->is_visible = true;
  self->wrap = false;
  self->need_relayout = true;
  self->need_repaint = true;
}

static void cst_render_context_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  CstRenderContext* self = CST_RENDER_CONTEXT(o);

  sys_clear_pointer(&self->width_calc, _sys_object_unref);
  sys_clear_pointer(&self->height_calc, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_render_context_parent_class)->dispose(o);
}

static void cst_render_context_class_init(CstRenderContextClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_render_context_dispose;

  cls->layout_self = cst_render_context_layout_self_i;
  cls->layout_children = cst_render_context_layout_children_i;
}
