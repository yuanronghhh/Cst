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

  FRRect bound;

  FRSInt4 border;
  FRSInt4 margin;
  FRSInt4 padding;
};

struct _CstLayoutNodeClass {
  SysObjectClass parent;

  CstLayoutNode* (*dclone) (CstLayoutNode *o);
  void (*layout) (CstLayoutNode* o, CstLayout* layout);
};

SysType cst_layout_node_get_type(void);
CstLayoutNode* cst_layout_node_new(void);
CstLayoutNode *cst_layout_node_new_I(void);

CstLayoutNode* cst_layout_node_clone(CstLayoutNode* olayout_node);
void cst_layout_node_set_width(CstLayoutNode* self, SysInt width);
SysInt cst_layout_node_get_width(CstLayoutNode* self);
void cst_layout_node_set_height(CstLayoutNode* self, SysInt width);
SysInt cst_layout_node_get_height(CstLayoutNode* self);

void cst_layout_node_set_x(CstLayoutNode* self, SysInt x);
SysInt cst_layout_node_get_x(CstLayoutNode* self);
void cst_layout_node_set_y(CstLayoutNode* self, SysInt width);
SysInt cst_layout_node_get_y(CstLayoutNode* self);

void cst_layout_node_set_xy(CstLayoutNode* self, SysInt x, SysInt y);
void cst_layout_node_set_size(CstLayoutNode* self, SysInt width, SysInt height);
void cst_layout_node_get_size(CstLayoutNode* self, SysInt *width, SysInt *height);

void cst_layout_node_maybe_expand(CstLayoutNode* self, CstRenderContext* ctx);
void cst_layout_node_get_mbp(CstLayoutNode* self, FRSInt4* m4);

const FRRect *cst_layout_node_get_bound(CstLayoutNode* self);
void cst_layout_node_set_bound(CstLayoutNode* self, const FRRect *bound);


void cst_layout_node_set_margin(CstLayoutNode *self, const FRSInt4 * margin);
const FRSInt4 * cst_layout_node_get_margin(CstLayoutNode *self);

void cst_layout_node_set_padding(CstLayoutNode *self, const FRSInt4 * padding);
const FRSInt4 * cst_layout_node_get_padding(CstLayoutNode *self);

void cst_layout_node_set_border(CstLayoutNode *self, const FRSInt4 * border);
const FRSInt4 * cst_layout_node_get_border(CstLayoutNode *self);

void cst_layout_node_fill_rectangle(CstLayoutNode *self, CstLayout* layout);
void cst_layout_node_stroke_rectangle(CstLayoutNode *self, CstLayout *layout);

void cst_layout_node_constraint_width(CstLayoutNode* self, CstRenderContext* rctx, CstRenderContext* pctx);
void cst_layout_node_constraint_height(CstLayoutNode* self, CstRenderContext* rctx, CstRenderContext* pctx);

void cst_layout_node_layout(CstLayoutNode * self, CstLayout * layout);

SYS_END_DECLS

#endif

