#include <CstCore/Front/Common/CstLayout.h>
#include <CstCore/Front/Common/CstNode.h>


struct _CstLayout {
  FRRect bound;
  SysList* nodes;
};

struct _CstLayoutContext {
  int direction;
};


/* layout */
void cst_layout_set_xy(CstLayout *self, SysInt x, SysInt y) {
  sys_return_if_fail(self != NULL);

  self->bound.x = x;
  self->bound.y = y;
}

SysBool cst_layout_need_wrap(CstLayout* self, SysInt append_width, SysInt max_width) {
  sys_return_val_if_fail(self != NULL, false);

  return self->bound.width + append_width > max_width;
}

CstLayout *cst_layout_new(SysInt x, SysInt y) {
  CstLayout *o = sys_new_N(CstLayout, 1);

  o->nodes = NULL;
  o->bound.x = x;
  o->bound.y = y;
  o->bound.width = 0;
  o->bound.height = 0;

  return o;
}

void cst_layout_free(CstLayout *self) {
  sys_return_if_fail(self != NULL);

  cst_layout_clear(self);

  sys_free_N(self);
}

void cst_layout_clear(CstLayout *self) {
  sys_return_if_fail(self != NULL);

  sys_list_free_full(self->nodes, (SysDestroyFunc)_sys_object_unref);
}

void cst_layout_layout_update(CstLayout* self) {
  sys_return_if_fail(self != NULL);

  SysInt w = 0, h = 0;
  CstNode* node;
  SysList* item = sys_list_last(self->nodes);

  sys_debug_N("%s", ">>> layout ");
  for (item = self->nodes; item; item = item->prev) {
    node = item->data;

    // cst_node_print_node(node);

    cst_node_get_size_mbp(node, &w, &h);
    cst_node_set_xy(node, self->bound.x + self->bound.width, self->bound.y);

    self->bound.width += w;
    self->bound.height = max(h, self->bound.height);
  }
  sys_debug_N("%s", "<<< layout");
}

void cst_layout_prepend_item(CstLayout *self, CstNode *node) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(node != NULL);

  self->nodes = sys_list_prepend(self->nodes, node);

  sys_object_ref(node);
}

void cst_layout_layout_nodes(CstLayout* layout, const FRRect* pbound, SysBool parent_wrap) {

  sys_return_if_fail(layout != NULL);

#if 0
  const SysChar* text;
  SysSList *layouts = layout;
  SysInt w = 0, h = 0;
  SysInt offset_w = 0;
  SysList *start = sys_list_last(layout->nodes);
  CstLayout *playout = layout;

  for (SysList* item = start; item; item = item->prev) {
    CstNode* v_node = item->data;

    cst_node_get_size_mbp(v_node, &w, &h);

    if (parent_wrap) {
      if (pbound->width == -1) {
        sys_warning_N("parent width must be set before wrap: %s,%s",
          cst_node_get_name(v_node),
          cst_node_get_id(v_node));
      }

      if (layout->bound.width > pbound->width) {
        layout->bound.width -= w;

        playout = cst_layout_new(pbound->x, pbound->y + h);
        cst_layout_prepend_data_h(playout, v_node);
        layouts = cst_layouts_prepend(layouts, playout);

        item->data = NULL;
        offset_w = 0;
      }
    }

    cst_node_set_xy(v_node,
      playout->bound.x + offset_w,
      playout->bound.y);

    offset_w += w;
  }

  for (SysList* item = start; item; item = item->prev) {
    if (item->data == NULL) {
      playout->nodes = sys_list_delete_link(playout->nodes, item);
    }
  }

  return layouts;
#endif
}

SysList* cst_layout_get_nodes(CstLayout * self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->nodes;
}

void cst_layout_set_size(CstLayout* self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  self->bound.width = width;
  self->bound.height = height;
}

void cst_layout_get_size(CstLayout* self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);

  *width = self->bound.width;
  *height = self->bound.height;
}

const FRRect* cst_layout_get_bound(CstLayout *self) {
  return &self->bound;
}

/* Layouts */
SysSList *cst_layouts_prepend(SysSList *layouts, CstLayout *layout) {
  layouts = sys_slist_prepend(layouts, layout);

  return layouts;
}

CstLayoutContext* cst_layout_context_new(void) {
  CstLayoutContext* o = sys_new0_N(CstLayoutContext, 1);

  return o;
}