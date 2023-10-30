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
  return sys_object_new(CST_TYPE_RENDER_CONTEXT, NULL);
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

SysObject* cst_render_context_dclone_i(SysObject* o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysObject* n = SYS_OBJECT_CLASS(cst_render_context_parent_class)->dclone(o);

  CstRenderContext *nself = CST_RENDER_CONTEXT(n);
  CstRenderContext *oself = CST_RENDER_CONTEXT(o);

  nself->need_relayout = oself->need_relayout;
  nself->need_repaint = oself->need_repaint;
  nself->is_visible = oself->is_visible;
  nself->wrap = oself->wrap;
  nself->line_space = oself->line_space;
  nself->child_count = oself->child_count;
  nself->mbp = oself->mbp;
  nself->prefer_height = oself->prefer_height;
  nself->prefer_width = oself->prefer_width;

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

SysBool cst_render_context_check_wrap(CstRenderContext* self, const FRRect *rbound) {
  sys_return_val_if_fail(self != NULL, false);

  if(!self->wrap) {
    return false;
  }

  if(self->prefer_width < rbound->width) {
    return true;
  }

  if(self->prefer_height < rbound->height) {
    return true;
  }

  return false;
}

void cst_render_context_inherit(CstRenderContext *self, CstRenderContext *pctx, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  if (pctx) {

    self->prefer_width = pctx->prefer_width;
    self->prefer_height = pctx->prefer_height;
  }
}

void cst_render_context_layout_self(CstRenderContext *self, CstLayerNode *node, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstRenderContextClass* cls = CST_RENDER_CONTEXT_GET_CLASS(self);
  sys_return_if_fail(cls->layout_self != NULL);

  cls->layout_self(self, node, layout);
}

void cst_render_context_layout_children(CstRenderContext *self, CstLayerNode *node, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstRenderContextClass* cls = CST_RENDER_CONTEXT_GET_CLASS(self);
  sys_return_if_fail(cls->layout_children != NULL);

  cls->layout_children(self, node, layout);
}

void cst_render_context_layout_self_i(CstRenderContext *self, CstLayerNode *node, CstLayout *layout) {
  SysInt w, h;
  CstLayoutNode *lnode;
  CstRenderNode *rnode;

  rnode = cst_layer_node_get_rnode(node);
  lnode = CST_LAYOUT_NODE(rnode);

  cst_layout_node_get_size(lnode, &w, &h);

  if (w == -1) {

    cst_layout_node_set_width(lnode, self->prefer_width);
  }

  if (h == -1) {

    cst_layout_node_set_height(lnode, self->prefer_height);
  }
}

void cst_render_context_layout_children_i(CstRenderContext *self, CstLayerNode *node, CstLayout *layout) {
}

void cst_render_context_layout_box_node(CstRenderContext* self, CstBoxNode *box, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

  CstRenderNode *rnode;
  CstRenderNode *crnode;
  CstLayerNode *clnode;
  CstLayerNode *lnode;
  CstBoxNode *bnode;
  CstRenderContext* cctx;

  lnode = CST_LAYER_NODE(box);
  rnode = cst_layer_node_get_rnode(lnode);

  cst_render_node_render_enter(rnode, layout);

  if(!self->need_relayout) {
    return;
  }

  if(!self->is_visible) {
    return;
  }

  cst_render_context_layout_self(self, lnode, layout);

  if (cst_box_node_children(box)) {
    clnode = CST_LAYER_NODE(cst_box_node_children(box));

    if (cst_box_node_has_one_child(box)) {
      crnode = cst_layer_node_get_rnode(clnode);
      cctx = cst_render_node_get_render_ctx(crnode);

      cst_render_context_inherit(cctx, self, layout);
      cst_render_context_layout_box_node(cctx, cst_box_node_children(box), layout);
    } else {

      for (bnode = cst_box_node_children(box); bnode; bnode = cst_box_node_next(box)) {
        cst_box_node_relayout_node(bnode, layout);
      }
    }
  }

  if (box->next) {

    cst_box_node_relayout_node(box->next, layout);
  }

  cst_render_node_render_leave(rnode, layout);
  self->need_relayout = false;
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
