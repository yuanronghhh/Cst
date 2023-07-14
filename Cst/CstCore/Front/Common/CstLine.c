#include <CstCore/Front/Common/CstLine.h>
#include <CstCore/Front/Common/CstNode.h>


struct _CstLine {
  FRRect bound;

  SysList* nodes;
  SysInt max_w;
  SysInt max_h;
};


/* line */
void cst_line_set_xy(CstLine *self, SysInt x, SysInt y) {
  sys_return_if_fail(self != NULL);

  self->bound.x = x;
  self->bound.y = y;
}

SysBool cst_line_need_wrap(CstLine* self, SysInt append_width, SysInt max_width) {
  sys_return_val_if_fail(self != NULL, false);

  return self->bound.width + append_width > max_width;
}

CstLine *cst_line_new(SysInt x, SysInt y) {
  CstLine *o = sys_new_N(CstLine, 1);

  o->nodes = NULL;
  o->bound.x = x;
  o->bound.y = y;
  o->bound.width = 0;
  o->bound.height = 0;
  o->max_w = 0;
  o->max_h = 0;

  return o;
}

void cst_line_free(CstLine *self) {
  sys_return_if_fail(self != NULL);

  cst_line_clear(self);

  sys_free_N(self);
}

void cst_line_clear(CstLine *self) {
  sys_return_if_fail(self != NULL);

  sys_list_free_full(self->nodes, (SysDestroyFunc)_sys_object_unref);
}

void cst_line_prepend_data_h(CstLine *self, CstNode *node) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(node != NULL);

  SysInt w = 0, h = 0;

  self->nodes = sys_list_prepend(self->nodes, node);

  cst_node_get_size_mbp(node, &w, &h);

  self->max_w = max(w, self->max_w);
  self->max_h = max(h, self->max_h);

  self->bound.width += w;
  self->bound.height = self->max_h;

  sys_object_ref(node);
}

void cst_line_layout_nodes(CstLine* line, CstNode *v_parent) {
  sys_return_if_fail(line != NULL);
  const FRRect* pbound;
  SysInt w = 0, h = 0;
  SysInt offset_w = 0;
  SysList* start = sys_list_last(line->nodes);

  pbound = cst_node_get_bound(v_parent);

  for (SysList* item = start; item; item = item->prev) {
    CstNode* v_node = item->data;

    cst_node_get_size_mbp(v_node, &w, &h);

    //if (cst_node_can_wrap(v_parent)) {
    //  if (pbound->width == -1) {
    //    sys_warning_N("parent width must be set before wrap: %s,%s",
    //      cst_node_get_name(v_node),
    //      cst_node_get_id(v_node));
    //  }

    //  if (cst_line_need_wrap(line, w, pbound->width)) {
    //    line = cst_line_new(pbound->x, pbound->x + h);
    //    offset_w = 0;
    //  }
    //}

    cst_node_set_xy(v_node,
      line->bound.x + offset_w,
      line->bound.y);

    offset_w += w;
  }
}

SysList* cst_line_get_nodes(CstLine * self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->nodes;
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

/* Lines */
SysSList *cst_lines_prepend(SysSList *lines, CstLine *line) {
  lines = sys_slist_prepend(lines, line);

  return lines;
}
