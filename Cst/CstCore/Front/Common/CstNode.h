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
SysBool cst_node_is_dirty(CstNode* node);
CstNode* cst_node_insert_after(CstNode *parent, CstNode *sibling, CstNode *node);
void cst_node_append(CstNode *parent, CstNode *node);

SysBool cst_node_is_visible(CstNode *node);
void cst_node_set_abs_node(CstNode* v_node, SysBool bvalue);

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
void cst_node_set_xy(CstNode* node, SysInt x, SysInt y);
SysInt cst_node_get_x(CstNode *node);
void cst_node_set_wrap(CstNode *node, SysBool wrap);
void cst_node_set_y(CstNode *node, SysInt width);
SysInt cst_node_get_y(CstNode *node);

void cst_node_init_mbp(CstNode *node);
void cst_node_layout(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state);
CstNode *cst_node_realize(CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render);
void cst_node_get_size_mbp(CstNode* node, SysInt* width, SysInt* height);
CstNode *cst_node_children(CstNode *node);
CstNode *cst_node_prev(CstNode *node);
CstNode *cst_node_next(CstNode *node);
FRAWatch * cst_node_get_awatch(CstNode * node, SysType atype, const SysChar * bind_var);
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
void cst_node_get_mbp(CstNode* node, FRSInt4* m4);
void cst_node_set_prefer_size(CstNode *node, SysInt width, SysInt height);
void cst_node_get_prefer_size(CstNode *node, SysInt *width, SysInt *height);
SysBool cst_node_can_wrap(CstNode *v_node);
SysSList* cst_node_get_lines(CstNode* v_node);
void cst_node_relayout_h(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state);
void cst_node_relayout_v(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state);
void cst_node_get_bound_mbp(CstNode *node, FRRect *rect_bp);
const FRRect *cst_node_get_bound(CstNode *node);
SysBool cst_node_is_abs_node(CstNode* node);
void cst_node_add_awatch(CstNode * node, FRAWatch * awatch);
void cst_node_stroke_rectangle(CstNode *node, FRContext *cr);
SysBool cst_node_set_css_r(CstNode *node, CstCssGroup *g);
void cst_node_realize_root(CstModule *v_module, CstComNode *ncomp_node, CstNode *root, CstNode *new_root, CstRender *v_render);
void cst_constrain_same_width(CstNode *v_parent, CstNode *v_node, FRContext *cr, SysPointer data);
void cst_constrain_same_height(CstNode *v_parent, CstNode *v_node, FRContext *cr, SysPointer data);

void cst_node_set_padding(CstNode* node, FRSInt4* m4);
void cst_node_get_padding(CstNode* node, FRSInt4* m4);
void cst_node_set_margin(CstNode* node, FRSInt4* m4);
void cst_node_get_margin(CstNode* node, FRSInt4* m4);

CstNode *cst_node_deep_clone(CstNode *v_node);
void cst_node_bind(CstNode *self, CstComNode *com_node);
void cst_node_construct(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props);
void cst_node_repaint(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state);
void cst_node_paint(CstModule * v_module, CstNode * v_parent, CstNode * v_node, FRContext * cr, FRDraw * draw, SysInt state);
void cst_node_relayout(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state);
void cst_node_relayout_down(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, FRContext *cr);
void cst_node_relayout_root(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state);
void cst_node_render_enter(CstNode *node, FRContext *cr, SysInt state);
void cst_node_render_leave(CstNode *node, FRContext *cr, SysInt state);
void cst_node_repaint_root(CstModule *v_module, CstNode *v_parent, CstNode *node, FRContext *cr, FRDraw *draw, SysInt state);

SYS_END_DECLS

#endif
