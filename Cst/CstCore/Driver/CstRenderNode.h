#ifndef __CST_RENDER_NODE__
#define __CST_RENDER_NODE__

#include <CstCore/Driver/CstLayoutNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_RENDER_NODE (cst_render_node_get_type())
#define CST_RENDER_NODE(o) ((CstRenderNode* )sys_object_cast_check(o, CST_TYPE_RENDER_NODE))
#define CST_RENDER_NODE_CLASS(o) ((CstRenderNodeClass *)sys_class_cast_check(o, CST_TYPE_RENDER_NODE))
#define CST_RENDER_NODE_GET_CLASS(o) sys_instance_get_class(o, CstRenderNodeClass)
#define CST_RENDER_NODE_NODE(o) ((o)->node)


struct _CstRenderNode {
  SysObject parent;
  /* < private > */

  /* ref node */
  CstNode *node;

  /* render context */
  CstRenderContext *render_ctx;
};

struct _CstRenderNodeClass {
  SysObjectClass parent;

  void (*construct) (CstRenderNode *self, CstNode* node);
  void (*relayout) (CstRenderNode* self, CstLayout* layout);
  void (*repaint) (CstRenderNode* self, CstLayout* layout);
  CstRenderNode* (*get_parent) (CstRenderNode* self);
};

SysType cst_render_node_get_type(void);
CstRenderNode *cst_render_node_new(void);
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

