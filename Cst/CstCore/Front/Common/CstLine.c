#include <CstCore/Front/Common/CstLinePrivate.h>
#include <CstCore/Front/Common/CstNode.h>


CstLine *cst_line_new(SysInt offset_w, SysInt offset_h) {
  CstLine *o = sys_new_N(CstLine, 1);

  o->nodes = NULL;
  o->offset_w = offset_w;
  o->offset_h = offset_h;

  return o;
}

void cst_line_free(CstLine *self) {
  sys_return_if_fail(self != NULL);

  sys_list_free_full(self->nodes, (SysDestroyFunc)_sys_object_unref);

  sys_free_N(self);
}

void cst_line_prepend_data(CstLine *self, CstNode *v_parent, CstNode* v_node) {
  sys_return_if_fail(self != NULL);

  FRRect bound = { 0 };
  const FRRect *pbound;
  SysInt m0, m1, m2, m3;

  self->nodes = sys_list_prepend(self->nodes, v_node);

  pbound = cst_node_get_bound(v_parent);
  cst_node_get_mbp(v_parent, &m0, &m1, &m2, &m3);

  cst_node_set_xy(v_node, 
    pbound->x + m3 + self->offset_w,
    pbound->y + m0 + self->offset_h);

  cst_node_get_bound_mbp(v_node, &bound);

  self->offset_w += bound.width;
}

SysBool cst_line_need_wrap (CstLine *self, SysInt append_width, SysInt width) {
  if(self->offset_w + append_width > width) {
    return true;
  }

  return false;
}

void cst_line_set_start(CstLine* self, SysInt x, SysInt y) {
  sys_return_if_fail(self != NULL);

  self->x = x;
  self->y = y;
}

void cst_line_get_offsize(CstLine* self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);

  *width = self->offset_w;
  *height = self->offset_h;
}

