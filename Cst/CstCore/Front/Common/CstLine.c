#include <CstCore/Front/Common/CstLine.h>
#include <CstCore/Front/Common/CstNode.h>


struct _CstLine {
  FRRect bound;

  SysList* nodes;
  SysInt max_w;
  SysInt max_h;
};

CstLine *cst_line_new(SysInt x, SysInt y) {
  CstLine *o = sys_new_N(CstLine, 1);

  o->nodes = NULL;
  o->bound.x = x;
  o->bound.y = y;
  o->max_w = 0;
  o->max_h = 0;
  
  return o;
}

void cst_line_free(CstLine *self) {
  sys_return_if_fail(self != NULL);

  sys_list_free_full(self->nodes, (SysDestroyFunc)_sys_object_unref);

  sys_free_N(self);
}

void cst_line_prepend_data(CstLine *self, CstNode *v_node) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_node != NULL);

  SysInt w = 0, h = 0;

  self->nodes = sys_list_prepend(self->nodes, v_node);
  cst_node_get_size_mbp(v_node, &w, &h);

  self->bound.x += w;

  self->max_w = max(w, self->max_w);
  self->max_h = max(h, self->max_h);

  sys_object_ref(v_node);
}

SysBool cst_line_need_wrap (CstLine *self, SysInt append_width, SysInt width) {
  if(self->bound.x + append_width > width) {
    return true;
  }

  return false;
}

void cst_line_get_maxsize(CstLine* self, SysInt* max_w, SysInt* max_h) {
  sys_return_if_fail(self != NULL);

  *max_w = self->max_w;
  *max_h = self->max_h;
}

void cst_line_set_size(CstLine* self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  self->bound.width = width;
  self->bound.height = height;
}

void cst_line_get_size(CstLine* self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);

  *width = self->bound.width;
  *height = self->bound.height;
}

const FRRect* cst_line_get_bound(CstLine *self) {
  return &self->bound;
}

SysSList *cst_lines_prepend(SysSList *lines, CstLine *line) {
  lines = sys_slist_prepend(lines, line);

  return lines;
}