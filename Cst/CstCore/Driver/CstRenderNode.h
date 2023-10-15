#ifndef __CST_RENDER_NODE__
#define __CST_RENDER_NODE__

#include <CstCore/Driver/CstLayoutNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_RENDER_NODE (cst_render_node_get_type())
#define CST_RENDER_NODE(o) ((CstRenderNode* )sys_object_cast_check(o, CST_TYPE_RENDER_NODE))
#define CST_RENDER_NODE_CLASS(o) ((CstRenderNodeClass *)sys_class_cast_check(o, CST_TYPE_RENDER_NODE))
#define CST_RENDER_NODE_GET_CLASS(o) sys_instance_get_class(o, CstRenderNodeClass)

struct _CstRenderNode {
  SysObject parent;
  /* < private > */

  /* ref node */
  CstNode *node;

};

struct _CstRenderNodeClass {
  CstLayoutNodeClass parent;

  void (*construct) (CstRenderNode *self, CstNode* node);
  void (*relayout) (CstRenderNode* self, CstLayout* layout);
  void (*repaint) (CstRenderNode* self, CstLayout* layout);
  CstRenderNode* (*get_parent)(CstRenderNode* self);
  CstRenderContext* (*get_default_ctx)(CstRenderNode *self);
};

SysType cst_render_node_get_type(void);
CstLayoutNode *cst_render_node_new(void);
CstLayoutNode *cst_render_node_new_I(CstNode *node);

CST_RENDER_NODE_ENUM cst_render_node_type_by_name(const SysChar* name);

const FRRect *cst_render_node_get_bound(CstRenderNode* self);
void cst_render_node_get_mbp(CstRenderNode * self, FRSInt4 * m4);


void cst_render_node_mark_dirty(CstRenderNode* self, SysBool bvalue);
SysBool cst_render_node_is_dirty(CstRenderNode* self);

SysBool cst_render_node_need_paint(CstRenderNode *self);
void cst_render_node_set_paint(CstRenderNode *self, SysBool bvalue);

SysBool cst_render_node_need_layout(CstRenderNode *self);
void cst_render_node_set_layout(CstRenderNode *self, SysBool bvalue);

SysBool cst_render_node_is_visible(CstRenderNode* self);
SysBool cst_render_node_can_wrap(CstRenderNode* self);
CstRenderNode* cst_render_node_get_parent(CstRenderNode* self);
const SysChar* cst_render_node_get_id(CstRenderNode *self);

const SysChar* cst_render_node_get_name(CstRenderNode *self);
void cst_render_node_set_name(CstRenderNode* self, const SysChar* name);

void cst_render_node_relayout_self(CstRenderNode *self, CstLayout *layout);
void cst_render_node_paint_self(CstRenderNode *self, CstLayout *layout);

void cst_render_node_fill_rectangle(CstRenderNode* self, CstLayout* layout);
void cst_render_node_stroke_rectangle(CstRenderNode* self, CstLayout* layout);

void cst_render_node_layout(CstRenderNode* self, CstLayout *layout);
void cst_render_node_constraint_size(CstRenderNode* self, CstRenderContext* pctx);
void cst_render_node_constraint_width(CstRenderNode * self, CstRenderContext * pctx);
void cst_render_node_constraint_height(CstRenderNode * self, CstRenderContext * pctx);

SysInt cst_render_node_check_dirty(CstRenderNode* rnode, FRRegion* region);
void cst_render_node_print(CstRenderNode * self);


/* css */
void cst_render_node_render_enter(CstRenderNode *self, CstLayout *layout);
void cst_render_node_render_leave(CstRenderNode *self, CstLayout *layout);

void cst_render_node_set_size(CstRenderNode* self, SysInt width, SysInt height);

void cst_render_node_set_render_ctx(CstRenderNode *self, CstRenderContext * render_ctx);
CstRenderContext * cst_render_node_get_render_ctx(CstRenderNode *self);

void cst_render_node_set_y(CstRenderNode *self, SysInt y);
SysInt cst_render_node_get_y(CstRenderNode *self);

void cst_render_node_set_x(CstRenderNode* self, SysInt x);
SysInt cst_render_node_get_x(CstRenderNode *self);

void cst_render_node_set_width(CstRenderNode* self, SysInt width);
void cst_render_node_set_height(CstRenderNode* self, SysInt height);

void cst_render_node_set_position(CstRenderNode *self, SysInt position);
SysInt cst_render_node_get_position(CstRenderNode *self);

void cst_render_node_prepare(CstRenderNode * self, CstLayout * layout);

void cst_render_node_set_margin(CstRenderNode *self, const FRSInt4 * margin);
const FRSInt4 * cst_render_node_get_margin(CstRenderNode *self);

void cst_render_node_set_border(CstRenderNode *self, const FRSInt4 * border);
const FRSInt4 * cst_render_node_get_border(CstRenderNode *self);

void cst_render_node_set_padding(CstRenderNode *self, const FRSInt4 * padding);
const FRSInt4 * cst_render_node_get_padding(CstRenderNode *self);

void cst_render_node_set_wrap(CstRenderNode *self, SysBool wrap);
SysBool cst_render_node_get_wrap(CstRenderNode *self);

SYS_END_DECLS

#endif

