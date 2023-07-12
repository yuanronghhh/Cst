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

void cst_line_layout_node_h(CstLine *self, CstNode *v_parent, CstNode *v_node) {
  sys_return_if_fail(self != NULL);

  const FRRect* bound;

  SysInt w = 0, h = 0;
  SysInt offset_w = 0;
  FRSInt4 m4;
  SysBool need_wrap;
  CstLine* line;

  bound = cst_node_get_bound(v_parent);
  need_wrap = cst_node_can_wrap(v_parent);
  line = self;

  cst_node_get_mbp(v_parent, &m4);
  cst_line_set_xy(line,
    bound->x + m4.m3,
    bound->y + m4.m0);

  for (SysList* item = self->nodes; item; item = item->next) {
    CstNode* node = item->data;

    if (need_wrap) {
      if (bound->width == -1) {
        sys_warning_N("parent width must be set before wrap: %s,%s", 
          cst_node_get_name(v_node), 
          cst_node_get_id(v_node));
      }
    }

    cst_node_get_size_mbp(node, &w, &h);

    if (self->bound.width + w > bound->width) {
      line = cst_line_new(line->bound.x, line->bound.y + h);
      offset_w = 0;
    }

    cst_node_set_xy(node, 
      line->bound.x + offset_w,
      line->bound.y + m4.m0);

    offset_w += w;
  }
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
