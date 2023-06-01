#ifndef __CST_NODE_H__
#define __CST_NODE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_NODE _CST_TYPE_NODE
#define CST_NODE(o) _CST_NODE(o)
#define CST_NODE_CLASS(o) _CST_NODE_CLASS(o)
#define CST_NODE_GET_CLASS(o) _CST_NODE_GET_CLASS(o)


struct _CstNodeProps {
  SysList *v_awatches;
  SysList *v_node_maps;

  const SysChar *v_id;
  SysChar **v_base;
  SysInt  v_base_len;
  SysBool v_absolute;
  SysChar *v_value;
  SysChar *v_label;
  SysInt  v_z_index;
  SysBool v_pass;
};

CstNode* cst_node_new(void);
SysType cst_node_get_type(void);

void cst_node_ref(CstNode *node);
void cst_node_unref(CstNode *node);
CstNode* cst_node_parent(CstNode *node);
CST_NODE_PROP_ENUM cst_node_prop_get_by_name(const SysChar *name);

void cst_node_unlink_node_r(CstNode *node);
void cst_node_relayout_reset(CstNode *node);
void cst_node_render_css(CstNode *node, FRContext *cr, CST_RENDER_STATE_ENUM state);
CstNode* cst_node_insert_after(CstNode *parent, CstNode *sibling, CstNode *node);
void cst_node_append(CstNode *parent, CstNode *node);

SysBool cst_node_get_is_dirty(CstNode *node);
void cst_node_set_is_dirty(CstNode *node, SysBool is_dirty);
SysBool cst_node_is_visible(CstNode *node);
void cst_node_set_layer(CstNode *v_node, SysInt layer);

void cst_node_print_node(CstNode *node);

void cst_node_set_width(CstNode *node, SysInt width);
SysInt cst_node_get_width(CstNode *node);
void cst_node_set_height(CstNode *node, SysInt width);
SysInt cst_node_get_height(CstNode *node);
void cst_node_set_name(CstNode *node, const SysChar *name);
const SysChar *cst_node_get_name(CstNode *node);

void cst_node_set_id(CstNode *node, const SysChar *id);
const SysChar *cst_node_get_id(CstNode *node);

void cst_node_set_x(CstNode *node, SysInt x);
SysInt cst_node_get_x(CstNode *node);
void cst_node_set_wrap(CstNode *node, SysBool wrap);
void cst_node_set_y(CstNode *node, SysInt width);
SysInt cst_node_get_y(CstNode *node);

void cst_node_get_padding(CstNode *node, SysInt16 *m0, SysInt16 *m1, SysInt16 *m2, SysInt16 *m3);
void cst_node_set_padding(CstNode *node, SysInt16 m0, SysInt16 m1, SysInt16 m2, SysInt16 m3);
void cst_node_get_margin(CstNode *node, SysInt16 *m0, SysInt16 *m1, SysInt16 *m2, SysInt16 *m3);
void cst_node_set_margin(CstNode *node, SysInt16 m0, SysInt16 m1, SysInt16 m2, SysInt16 m3);

void cst_node_set_mbp(CstNode *node);
void cst_node_layout(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state);
CstNode *cst_node_realize(CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render);
CstNode *cst_node_children(CstNode *node);
CstNode *cst_node_prev(CstNode *node);
CstNode *cst_node_next(CstNode *node);
CstNode *cst_node_get_last_child(CstNode *node);
void cst_node_set_last_child(CstNode *node, CstNode *last_child);
FRPrioList *cst_node_get_abs_node(CstNode *node);
SysBool cst_node_get_need_relayout(CstNode *node);
SysBool cst_node_get_need_repaint(CstNode *node);
void cst_node_set_need_relayout(CstNode *node, SysBool need_relayout);
void cst_node_set_need_repaint(CstNode * node, SysBool need_relayout);
void cst_node_set_width_closure(CstNode *node, CstCssClosure *c);
void cst_node_set_height_closure(CstNode *node, CstCssClosure *c);
void cst_node_set_child_width_closure(CstNode *node, CstCssClosure *c);
void cst_node_set_child_height_closure(CstNode *node, CstCssClosure *c);
void cst_node_set_size(CstNode *node, SysInt width, SysInt height);
void cst_node_get_size(CstNode *node, SysInt *width, SysInt *height);
void cst_node_get_mbp(CstNode *node, SysInt *m0, SysInt *m1, SysInt *m2, SysInt *m3);
void cst_node_set_prefer_size(CstNode *node, SysInt width, SysInt height);
void cst_node_get_prefer_size(CstNode *node, SysInt *width, SysInt *height);
SysBool cst_node_can_wrap(CstNode *v_node);
void cst_node_relayout_h(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state);
void cst_node_relayout_v(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state);
void cst_node_get_bound_bp(CstNode *node, FRRect *rect_bp);
const FRRect *cst_node_get_bound(CstNode *node);
void cst_node_set_bound(CstNode *node, const FRRect *rect);
SysBool cst_node_layer_has_flag(CstNode *node, SysInt flag);
void cst_node_stroke_rectangle(CstNode *node, FRContext *cr);
SysBool cst_node_set_css_r(CstNode *node, CstCssGroup *g);
void cst_node_realize_root(CstModule *v_module, CstComNode *ncomp_node, CstNode *root, CstNode *new_root, CstRender *v_render);
void cst_constrain_same_width(CstNode *v_parent, CstNode *v_node, FRContext *cr, SysPointer data);
void cst_constrain_same_height(CstNode *v_parent, CstNode *v_node, FRContext *cr, SysPointer data);

CstNode *cst_node_deep_clone(CstNode *v_node);
void cst_node_bind(CstNode *self, CstComNode *com_node);
void cst_node_construct(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props);
void cst_node_repaint(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state);
void cst_node_relayout(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state);
void cst_node_relayout_down(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, FRContext *cr);
void cst_node_relayout_root(CstModule *v_module, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state);
void cst_node_render_enter(CstNode *node, FRContext *cr, SysInt state);
void cst_node_render_leave(CstNode *node, FRContext *cr, SysInt state);
void cst_node_paint_root(CstModule *v_module, CstNode *node, FRContext *cr, FRDraw *draw, SysInt state);

SYS_END_DECLS

#endif
