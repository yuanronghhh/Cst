#include <CstCore/Driver/CstLayoutContext.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/CstLayoutNode.h>

SYS_DEFINE_TYPE(CstLayoutContext, cst_layout_context, SYS_TYPE_OBJECT);


CstLayoutContext* cst_layout_context_new(void) {
  return sys_object_new(CST_TYPE_LAYOUT_CONTEXT, NULL);
}

void cst_layout_context_set_mbp(CstLayoutContext* self, FRSInt4* m4) {
  sys_return_if_fail(self != NULL);

  mbp.m0 = m4->m0;
  mbp.m1 = m4->m1;
  mbp.m2 = m4->m2;
  mbp.m3 = m4->m3;
}

const FRSInt4* cst_layout_context_get_mbp(CstLayoutContext* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return &mbp;
}

void cst_layout_context_set_child_width_closure(CstLayoutContext* self, CstCssClosure* c) {
  sys_return_if_fail(self != NULL);

  if (self->child_width_calc) {
    cst_css_closure_unref(self->child_width_calc);
  }

  self->child_width_calc = c;
}

void cst_layout_context_set_child_height_closure(CstLayoutContext* self, CstCssClosure* c) {
  sys_return_if_fail(self != NULL);

  if (self->child_height_calc) {
    cst_css_closure_unref(self->child_height_calc);
  }

  self->child_height_calc = c;
}

void cst_layout_context_set_width_closure(CstLayoutContext* self, CstCssClosure* c) {
  sys_return_if_fail(self != NULL);

  if (self->width_calc) {
    cst_css_closure_unref(self->width_calc);
  }

  self->width_calc = c;
}

void cst_layout_context_set_height_closure(CstLayoutContext* self, CstCssClosure* c) {
  sys_return_if_fail(self != NULL);

  if (self->height_calc) {
    cst_css_closure_unref(self->height_calc);
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

void cst_layout_context_calc_size(CstLayoutContext *self, CstLayoutNode *layout_node) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(layout_node != NULL);

  cst_css_closure_calc(self->width_calc, layout_node, self, NULL, NULL);
  cst_css_closure_calc(self->height_calc, layout_node, self, NULL, NULL);
}

SysBool cst_layout_context_can_wrap(CstLayoutContext* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->wrap;
}

void cst_layout_context_layout_self(CstLayoutContext* self, CstLayoutContext* pself, CstLayoutNode *layout_node, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

  SysInt w = 0, h = 0;
  FRSInt4 *mbp = &self->mbp;

  cst_layout_node_get_size(layout_node, &w, &h);

  w = w + mbp->m1 + mbp->m3;
  h = h + mbp->m2 + mbp->m0;

  if (pself->child_width_calc) {
    pself->prefer_width = max(w, pself->prefer_width);
    cst_css_closure_calc(pself->child_width_calc, layout_node, self, NULL, NULL);
  }

  if (pself->child_height_calc) {
    pself->prefer_height = max(h, pself->prefer_height);
    cst_css_closure_calc(pself->child_height_calc, layout_node, self, NULL, NULL);
  }

  pself->prefer_width += w;
  pself->prefer_height = max(h, pself->prefer_height);
}

void cst_layout_context_layout_children(CstLayoutContext* self, CstLayoutContext* pself, CstLayoutNode* layout_node, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

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
