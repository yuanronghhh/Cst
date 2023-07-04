#include <CstCore/Front/Common/CstLinePrivate.h>
#include <CstCore/Front/Common/CstNode.h>


CstLine *cst_line_new(void) {
  CstLine *o = sys_new_N(CstLine, 1);

  o->nodes = NULL;
  o->offset_h = 0;
  o->offset_w = 0;

  return o;
}

void cst_line_free(CstLine *self) {
  sys_return_if_fail(self != NULL);

  sys_list_free_full(self->nodes, (SysDestroyFunc)_sys_object_unref);

  sys_free_N(self);
}

void cst_line_prepend(CstLine *self, CstNode *v_parent, CstNode* v_node) {
  sys_return_if_fail(self != NULL);

  FRRect bound;

  self->nodes = sys_list_prepend(self->nodes, v_node);

  cst_node_set_x(v_node, self->offset_w);
  cst_node_get_bound_mbp(v_node, &bound);

  self->offset_w += bound.width;

  bound.x = self->x + self->offset_w;
  bound.y = self->y + self->offset_h;
}

void cst_line_set_start(CstLine* self, SysInt x, SysInt y) {
  sys_return_if_fail(self != NULL);

  self->x = x;
  self->y = y;
}