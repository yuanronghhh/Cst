#ifndef __CST_RENDER_NODE__
#define __CST_RENDER_NODE__

#include <CstCore/Driver/CstCommon.h>

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

  /* render_node */
  CstLayoutNode *layout_node;
  CstLayoutContext *layout_ctx;
  SysPtrArray *css_groups;
};

struct _CstRenderNodeClass {
  SysObjectClass parent;

  void (*construct)(CstRenderNode *self, CstNode *node);
  CstRenderNode* (*get_parent)(CstRenderNode *self);
};

SYS_API SysType cst_render_node_get_type(void);
SYS_API CstRenderNode *cst_render_node_new(void);
SYS_API CstRenderNode *cst_render_node_new_I(CstNode *node);

void cst_render_node_prepare(CstRenderNode *self);
void cst_render_node_get_mbp(CstRenderNode* self, FRSInt4* m4);

void cst_render_node_set_abs_node(CstRenderNode *self, SysBool bvalue);
SysBool cst_render_node_is_abs_node(CstRenderNode *self);
const FRRect *cst_render_node_get_bound(CstRenderNode* self);
void cst_render_node_set_x(CstRenderNode* self, SysInt x);
void cst_render_node_set_width(CstRenderNode* self, SysInt width);
void cst_render_node_set_height(CstRenderNode* self, SysInt height);

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

void cst_render_node_relayout_self(CstRenderNode *self, CstLayout *layout);
void cst_render_node_paint_self(CstRenderNode *self, CstLayout *layout);

/* css */
SysBool cst_render_node_set_css_r(CstRenderNode *self, CstCssGroup *g);
void cst_render_node_render_enter(CstRenderNode *self, CstLayout *layout);
void cst_render_node_render_leave(CstRenderNode *self, CstLayout *layout);

SYS_END_DECLS

#endif

