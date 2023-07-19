#include <CstCore/Front/Common/CstLine.h>
#include <CstCore/Front/Common/CstNode.h>
#include <CstCore/Front/Common/CstText.h>


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

SysSList *cst_line_layout_nodes(CstLine* line, SysSList *lines, const FRRect* pbound, SysBool parent_wrap) {

  sys_return_val_if_fail(line != NULL, NULL);

  const SysChar* text;
  SysInt w = 0, h = 0;
  SysInt offset_w = 0;
  SysList *start = sys_list_last(line->nodes);
  CstLine *pline = line;

  sys_debug_N("%s", "start");
  for (SysList* item = start; item; item = item->prev) {
    CstNode* v_node = item->data;
    sys_debug_N("id: %s,%s", cst_node_get_name(v_node), cst_node_get_id(v_node));

    cst_node_get_size_mbp(v_node, &w, &h);

    if (parent_wrap) {
      if (pbound->width == -1) {
        sys_warning_N("parent width must be set before wrap: %s,%s",
          cst_node_get_name(v_node),
          cst_node_get_id(v_node));
      }

      if (line->bound.width > pbound->width) {
        line->bound.width = -1;

        pline = cst_line_new(pbound->x, pbound->y + h);

        sys_debug_N("wrap: %s,%s", cst_node_get_name(v_node), cst_node_get_id(v_node));
        cst_line_prepend_data_h(pline, v_node);
        
        lines = cst_lines_prepend(lines, pline);

        item->data = NULL;
        offset_w = 0;

        text = cst_text_get_text(CST_TEXT(cst_node_children(v_node)));
        sys_debug_N("%s", text);
      }
    }

    cst_node_set_xy(v_node,
      pline->bound.x + offset_w,
      pline->bound.y);

    offset_w += w;
  }

  for (SysList* item = start; item; item = item->prev) {
    if (item->data == NULL) {
      pline->nodes = sys_list_delete_link(pline->nodes, item);
    }
  }

  return lines;
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
