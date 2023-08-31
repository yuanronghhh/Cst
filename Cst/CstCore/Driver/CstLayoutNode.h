#ifndef __CST_LAYOUT_NODE__
#define __CST_LAYOUT_NODE__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_LAYOUT_NODE (cst_layout_node_get_type())
#define CST_LAYOUT_NODE(o) ((CstLayoutNode* )sys_object_cast_check(o, CST_TYPE_LAYOUT_NODE))
#define CST_LAYOUT_NODE_CLASS(o) ((CstLayoutNodeClass *)sys_class_cast_check(o, CST_TYPE_LAYOUT_NODE))
#define CST_LAYOUT_NODE_GET_CLASS(o) sys_instance_get_class(o, CstLayoutNodeClass)

struct _CstLayoutNode {
  SysObject parent;

  /* < private > */
  SysInt x;
  SysInt y;
  SysInt width;
  SysInt height;

  SysBool need_relayout;
  SysBool need_repaint;
  SysBool is_visible;
  SysBool wrap;
  FRRect bound;
  SysInt16 line_space;
  FRSInt4 border;
  FRSInt4 margin;
  FRSInt4 padding;

  SysInt child_count;
  FRSInt4 mbp;

  SysInt prefer_height;
  SysInt prefer_width;

  // self constraint
  CstCssClosure  *width_calc;
  CstCssClosure  *height_calc;

  // constraint for child, may be NULL.
  CstCssClosure  *child_width_calc;
  CstCssClosure  *child_height_calc;
};

struct _CstLayoutNodeClass {
  SysObjectClass parent;

  void (*relayout) (CstModule *v_module, CstLayoutNode *v_parent, CstLayoutNode *v_node, FRDraw *draw, CstLayout *layout);
  void (*relayout_down) (CstModule *v_module, CstLayoutNode *v_parent, CstLayoutNode *v_node, FRDraw *draw, CstLayout *layout);
};

SysType cst_layout_node_get_type(void);
CstLayoutNode *cst_layout_node_new(void);
CstLayoutNode *cst_layout_node_new_I(SysInt x, SysInt y, SysInt width, SysInt height);
CstLayoutNode* cst_layout_node_clone(CstLayoutNode* olayout_node);

void cst_layout_node_set_size(CstLayoutNode* self, SysInt width, SysInt height);
void cst_layout_node_get_size(CstLayoutNode* self, SysInt *width, SysInt *height);

void cst_layout_node_set_width(CstLayoutNode* self, SysInt width);
SysInt cst_layout_node_get_width(CstLayoutNode* self);
void cst_layout_node_set_height(CstLayoutNode* self, SysInt width);
SysInt cst_layout_node_get_height(CstLayoutNode* self);

void cst_layout_node_set_x(CstLayoutNode* self, SysInt x);
void cst_layout_node_set_xy(CstLayoutNode* self, SysInt x, SysInt y);
SysInt cst_layout_node_get_x(CstLayoutNode* self);
void cst_layout_node_set_y(CstLayoutNode* self, SysInt width);
SysInt cst_layout_node_get_y(CstLayoutNode* self);

void cst_layout_node_relayout(CstModule *v_module, CstLayoutNode *v_parent, CstLayoutNode *v_layout_node, FRDraw *draw, CstLayout *layout);
void cst_layout_node_relayout_down(CstModule *v_module, CstLayoutNode *v_parent, CstLayoutNode *self, FRDraw *draw, CstLayout *layout);

SYS_END_DECLS

#endif

