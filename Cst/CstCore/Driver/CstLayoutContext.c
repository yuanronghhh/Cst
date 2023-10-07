#include <CstCore/Driver/CstLayoutContext.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/Css/CstCssClosure.h>
#include <CstCore/Driver/CstLayoutNode.h>

SYS_DEFINE_TYPE(CstLayoutContext, cst_layout_context, SYS_TYPE_OBJECT);


CstLayoutContext* cst_layout_context_new(void) {
  return sys_object_new(CST_TYPE_LAYOUT_CONTEXT, NULL);
}

void cst_layout_context_set_mbp(CstLayoutContext* self, FRSInt4* m4) {
  sys_return_if_fail(self != NULL);

  self->mbp.m0 = m4->m0;
  self->mbp.m1 = m4->m1;
  self->mbp.m2 = m4->m2;
  self->mbp.m3 = m4->m3;
}

const FRSInt4* cst_layout_context_get_mbp(CstLayoutContext* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return &self->mbp;
}

SysBool cst_layout_context_is_abs_node(CstLayoutContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->node_type & CST_RENDER_NODE_ABS;
}

void cst_layout_context_set_abs_node(CstLayoutContext* self, SysInt bvalue) {
  sys_return_if_fail(self != NULL);

  self->node_type &= bvalue;
}

SysBool cst_layout_context_is_box_node(CstLayoutContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->node_type & CST_RENDER_NODE_BOX;
}

void cst_layout_context_set_node_type(CstLayoutContext *self, SysInt flag) {
  sys_return_if_fail(self != NULL);

  self->node_type = flag;
}

void cst_layout_context_set_child_width_closure(CstLayoutContext* self, CstCssClosure* c) {
  sys_return_if_fail(self != NULL);

  if (self->child_width_calc) {
    sys_clear_pointer(&self->child_width_calc, (SysDestroyFunc)_sys_object_unref);
  }

  self->child_width_calc = c;
}

void cst_layout_context_set_child_height_closure(CstLayoutContext* self, CstCssClosure* c) {
  sys_return_if_fail(self != NULL);

  if (self->child_height_calc) {
    sys_clear_pointer(&self->child_height_calc, (SysDestroyFunc)_sys_object_unref);
  }

  self->child_height_calc = c;
}

void cst_layout_context_set_width_closure(CstLayoutContext* self, CstCssClosure* c) {
  sys_return_if_fail(self != NULL);

  if (self->width_calc) {
    sys_clear_pointer(&self->width_calc, (SysDestroyFunc)_sys_object_unref);
  }

  self->width_calc = c;
}

void cst_layout_context_set_height_closure(CstLayoutContext* self, CstCssClosure* c) {
  sys_return_if_fail(self != NULL);

  if (self->height_calc) {
    sys_clear_pointer(&self->height_calc, (SysDestroyFunc)_sys_object_unref);
  }

  self->height_calc = c;
}

void cst_layout_context_set_prefer_size(CstLayoutContext* self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  self->prefer_width = width;
  self->prefer_height = height;
}

void cst_layout_context_get_prefer_size(CstLayoutContext* self, SysInt* width, SysInt* height) {
  sys_return_if_fail(self != NULL);

  *width = self->prefer_width;
  *height = self->prefer_height;
}

SysBool cst_layout_context_need_layout(CstLayoutContext *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->need_relayout;
}

void cst_layout_context_set_layout(CstLayoutContext *self, SysBool bvalue) {
  sys_return_if_fail(self != NULL);

  self->need_relayout = bvalue;
}

void cst_layout_context_calc_size(CstLayoutContext *self, CstRenderNode *render_node) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(render_node != NULL);

  cst_css_closure_calc(self->width_calc, render_node);
  cst_css_closure_calc(self->height_calc, render_node);
}

SysBool cst_layout_context_can_wrap(CstLayoutContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->wrap;
}

void cst_layout_context_layout_self(CstLayoutContext* self, CstRenderNode *render_node, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

  CstLayoutContextClass *cls = CST_LAYOUT_CONTEXT_GET_CLASS(self);
  sys_return_if_fail(cls->layout_self != NULL);

  cls->layout_self(self, render_node, layout);
}

void cst_layout_context_layout_children(CstLayoutContext* self, CstRenderNode *render_node, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

  CstLayoutContextClass *cls = CST_LAYOUT_CONTEXT_GET_CLASS(self);
  sys_return_if_fail(cls->layout_children != NULL);

  cls->layout_children(self, render_node, layout);
}

CstLayoutContext* cst_layout_context_dclone(CstLayoutContext *o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysType type = sys_type_from_instance(o);
  CstLayoutContext *n = sys_object_new(type, NULL);

  n->prefer_height = o->prefer_height;
  n->prefer_width = o->prefer_width;
  n->need_relayout = o->need_relayout;
  // TODO

  return n;
}

void cst_layout_context_layout_self_i(CstLayoutContext* self, CstRenderNode *render_node, CstLayout* layout) {
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

/* paint */
SysBool cst_layout_context_need_paint(CstLayoutContext *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->need_repaint;
}

void cst_layout_context_set_paint(CstLayoutContext *self, SysBool bvalue) {
  sys_return_if_fail(self != NULL);

  self->need_repaint = bvalue;
}

SysBool cst_layout_context_is_visible(CstLayoutContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->is_visible;
}

/* object api */
CstLayoutContext *cst_layout_context_new_I(void) {
  CstLayoutContext *o = cst_layout_context_new();
  return o;
}

static void cst_layout_context_init(CstLayoutContext *self) {
}

static void cst_layout_context_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  SYS_OBJECT_CLASS(cst_layout_context_parent_class)->dispose(o);
}

static void cst_layout_context_class_init(CstLayoutContextClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layout_context_dispose;
}
