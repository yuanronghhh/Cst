#ifndef __CST_RENDER_NODE__
#define __CST_RENDER_NODE__

#include <CstCore/Driver/CstNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_RENDER_NODE CST_TYPE_NODE
#define CST_RENDER_NODE(o) CST_NODE(o)
#define CST_RENDER_NODE_CLASS(o) CST_NODE_CLASS(o)
#define CST_RENDER_NODE_GET_CLASS(o) CST_NODE_GET_CLASS(o)
#define CST_RENDER_NODE_NODE(o) CST_NODE(o)

#define cst_render_node_get_type() cst_node_get_type()
CstRenderNode *cst_render_node_new_I(CstNode *node);

CST_RENDER_NODE_ENUM cst_render_node_type_by_name(const SysChar* name);
CstRenderNode* cst_render_node_get_parent(CstRenderNode* self);
void cst_render_node_print(CstRenderNode * self);


/* css */
void cst_render_node_render_enter(CstRenderNode *self, CstLayout *layout);
void cst_render_node_render_leave(CstRenderNode *self, CstLayout *layout);

void cst_render_node_set_render_ctx(CstRenderNode *self, CstRenderContext * render_ctx);
CstRenderContext * cst_render_node_get_render_ctx(CstRenderNode *self);

CstLayoutNode* cst_render_node_get_lnode(CstRenderNode* self);
void cst_render_node_prepare(CstRenderNode * self, CstLayout * layout);
CstNode * cst_render_node_get_node(CstRenderNode *self);
SysType cst_render_node_get_node_type(CstRenderNode *self);

SYS_END_DECLS

#endif

