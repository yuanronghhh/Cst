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

void cst_node_stroke_rectangle(CstNode *node, FRContext *cr) {
  const FRRect *rect;

  SysInt x, y, width, height;
  SysInt16 m0, m1, m2, m3;
  SysInt16 p0, p1, p2, p3;

  rect = cst_node_get_bound(node);
  cst_node_get_margin(node, &m0, &m1, &m2, &m3);
  cst_node_get_padding(node, &p0, &p1, &p2, &p3);

  x = rect->x + m3;
  y = rect->y + m0;
  width = rect->width + p1 + p3;
  height = rect->height + p0 + p2;

  fr_context_rectangle(cr, x, y, width, height);
  fr_context_stroke(cr);
}

/* object api */
void cst_node_relayout_root(CstModule *v_module, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state) {
  sys_return_if_fail(v_node != NULL);

  CstNode *v_children = cst_node_children(v_node);

  cst_node_render_enter(v_node, cr, state);
  cst_node_relayout(v_module, NULL, v_node, cr, draw, state);

  if(v_children) {
    cst_node_layout(v_module, v_node, v_children, cr, draw, state);
  }

  cst_node_render_leave(v_node, cr, state);
  cst_node_set_need_relayout(v_node, false);
}

static void node_repaint_node_r(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state) {
  sys_return_if_fail(v_node != NULL);

  CstNode *v_children = cst_node_children(v_node);
  CstNode *v_next = cst_node_next(v_node);

  cst_node_render_enter(v_node, cr, state);
  
  cst_node_repaint(v_module, v_parent, v_node, cr, draw, state);
  
  cst_node_render_leave(v_node, cr, state);
  cst_node_set_need_repaint(v_node, false);

  if (v_children) {
    node_repaint_node_r(v_module, v_node, v_children, cr, draw, state);
  }

  if (v_next) {
    node_repaint_node_r(v_module, v_parent, v_next, cr, draw, state);
  }
}

void cst_node_paint_root(CstModule *v_module, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state) {

  node_repaint_node_r(v_module, NULL, v_node, cr, draw, state);
}

void cst_node_render_enter(CstNode *node, FRContext *cr, SysInt state) {
  sys_return_if_fail(node != NULL);
  sys_return_if_fail(cr != NULL);

  fr_context_save(cr);
  cst_node_render_css(node, cr, state);

  if (state == CST_RENDER_STATE_RELAYOUT) {
    cst_node_relayout_reset(node);
  }
}

void cst_node_render_leave(CstNode *node, FRContext *cr, SysInt state) {
  sys_return_if_fail(node != NULL);
  sys_return_if_fail(cr != NULL);

  fr_context_restore(cr);
}

