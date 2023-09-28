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

CstNode* cst_node_parent(CstNode *node);
CST_NODE_PROP_ENUM cst_node_prop_get_by_name(const SysChar *name);

void cst_node_unlink_node_r(CstNode *node);
void cst_node_render_css(CstNode *node, FRContext *cr, CstLayout *layout);
void cst_node_set_abs_link(CstNode* self, FRPrioLink* nlink);

SysBool cst_node_is_dirty(CstNode* node);
CstNode* cst_node_insert_after(CstNode *parent, CstNode *sibling, CstNode *node);
void cst_node_append(CstNode *parent, CstNode *node);

SysBool cst_node_is_visible(CstNode *node);

void cst_node_print_node_r(CstNode* node);
void cst_node_print_node(CstNode *node);

void cst_node_set_name(CstNode *node, const SysChar *name);
const SysChar *cst_node_get_name(CstNode *node);

void cst_node_set_id(CstNode *node, const SysChar *id);
const SysChar *cst_node_get_id(CstNode *node);

CstNode *cst_node_realize(CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render);
CstNode *cst_node_children(CstNode *node);
CstNode *cst_node_prev(CstNode *node);
CstNode *cst_node_next(CstNode *node);
FRAWatch *cst_node_get_awatch(CstNode *node, SysType atype, const SysChar *bind_var);
CstNode *cst_node_get_last_child(CstNode *node);
void cst_node_set_last_child(CstNode *node, CstNode *last_child);
FRPrioLink *cst_node_get_abs_node(CstNode *node);
SysBool cst_node_can_wrap(CstNode *v_node);
void cst_node_relayout_v(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRDraw *draw, CstLayout *layout);
SysBool cst_node_is_abs_node(CstNode* node);
void cst_node_add_awatch(CstNode *node, FRAWatch *awatch);
void cst_node_stroke_rectangle(CstNode *node, FRDraw *draw);
void cst_node_fill_rectangle(CstNode *node, FRDraw *draw);
CstLayoutContext *cst_node_get_layout_node(CstNode *self);
SysBool cst_node_set_css_r(CstNode *node, CstCssGroup *g);
void cst_node_realize_root(CstModule *v_module, CstComNode *ncomp_node, CstNode *root, CstNode *new_root, CstRender *v_render);
void cst_constrain_same_width(CstNode *v_parent, CstNode *v_node, FRContext *cr, SysPointer data);
void cst_constrain_same_height(CstNode *v_parent, CstNode *v_node, FRContext *cr, SysPointer data);

CstNode *cst_node_deep_clone(CstNode *v_node);
void cst_node_bind(CstNode *self, CstComNode *com_node);
void cst_node_construct(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props);
void cst_node_paint(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRDraw *draw, CstLayout *layout);
void cst_node_relayout_root(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRDraw *draw, CstLayout *layout);
void cst_node_render_enter(CstNode *node, FRContext *cr, CstLayout *layout);
void cst_node_render_leave(CstNode *node, FRContext *cr, CstLayout *layout);
void cst_node_repaint_root(CstModule *v_module, CstNode *v_parent, CstNode *node, FRDraw *draw, CstLayout *layout);

SYS_END_DECLS

#endif
