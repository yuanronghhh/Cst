#include "CstNode.h"
#include <CstCore/Front/Common/CstNodePrivate.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstModule.h>

#define ID_FORMAT "id.%d.%d"

static const SysChar* CST_NODE_PROP_NAMES[] = {
  "id","value","base","absolute", "label",
  "key_up","key_down",
};

CST_NODE_PROP_ENUM cst_node_prop_get_by_name(const SysChar * name) {
  sys_return_val_if_fail(name != NULL, -1);

  const SysChar* item = CST_NODE_PROP_NAMES[0];
  SysInt i;
  for (i = 0; i < ARRAY_SIZE(CST_NODE_PROP_NAMES); i++) {
    item = CST_NODE_PROP_NAMES[i];

    if (*name == '@') {
      return CST_NODE_PROP_ACTION;
    }

    if (sys_str_equal(name, item)) {
      return i;
    }
  }

  return CST_NODE_PROP_LAST;
}

void cst_node_stroke_rectangle(CstNode *node, FRDraw *draw) {
  FRSInt4 m4, p4;
  const FRRect* bound;

  cst_node_get_margin(node, &m4);
  cst_node_get_padding(node, &p4);

  bound = cst_node_get_bound(node);

  fr_draw_stroke_mp(draw, bound, &m4, &p4);

  sys_debug_N("repaint node: %s,%s<%d,%d,%d,%d>",
      cst_node_get_id(node),
      cst_node_get_name(node),
      bound->x,
      bound->y,
      bound->width,
      bound->height);
}

void cst_node_fill_rectangle(CstNode *node, FRDraw *draw) {
  const FRRect* bound = cst_node_get_bound(node);

  fr_draw_fill_rectangle(draw, bound);
}

/* object api */
void cst_node_relayout_root(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(v_node != NULL);

  CstNode *v_children = cst_node_children(v_node);
  FRContext *cr = fr_draw_get_cr(draw);

  cst_node_render_enter(v_node, cr, layout);
  cst_node_relayout(v_module, v_parent, v_node, draw, layout);

  if(v_children) {
    cst_node_layout(v_module, v_node, v_children, draw, layout);
    cst_node_layout_down(v_module, v_node, v_children, draw, layout);
  }

  cst_node_render_leave(v_node, cr, layout);
  cst_node_set_need_relayout(v_node, false);
}

void cst_node_paint(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(v_node != NULL);

  FRContext *cr = fr_draw_get_cr(draw);

  if (!cst_node_get_need_repaint(v_node)) {
    sys_warning_N("%s", "repaint should not execute twice.");
    return;
  }

  cst_node_render_enter(v_node, cr, layout);
  cst_node_repaint(v_module, v_parent, v_node, draw, layout);
  cst_node_render_leave(v_node, cr, layout);

  cst_node_set_need_repaint(v_node, false);
}

static void node_repaint_node_r(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(v_node != NULL);

  CstNode *v_children = cst_node_children(v_node);
  CstNode *v_next = cst_node_next(v_node);

  cst_node_paint(v_module, v_parent, v_node, draw, layout);

  if (v_children) {
    node_repaint_node_r(v_module, v_node, v_children, draw, layout);
  }

  if (v_next) {
    node_repaint_node_r(v_module, v_parent, v_next, draw, layout);
  }
}

void cst_node_repaint_root(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRDraw *draw, CstLayout *layout) {

  node_repaint_node_r(v_module, v_parent, v_node, draw, layout);
}

void cst_node_render_enter(CstNode *node, FRContext *cr, CstLayout *layout) {
  sys_return_if_fail(node != NULL);
  sys_return_if_fail(cr != NULL);

  fr_context_save(cr);
  cst_node_render_css(node, cr, layout);
}

void cst_node_render_leave(CstNode *node, FRContext *cr, CstLayout *layout) {
  sys_return_if_fail(node != NULL);
  sys_return_if_fail(cr != NULL);

  fr_context_restore(cr);
}

