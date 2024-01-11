#ifndef __CST_RENDER_NODE__
#define __CST_RENDER_NODE__

#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstRenderContext.h>

SYS_BEGIN_DECLS

#define CST_TYPE_RENDER_NODE (cst_render_node_get_type())
#define CST_RENDER_NODE(o) ((CstRenderNode* )sys_object_cast_check(o, CST_TYPE_RENDER_NODE))
#define CST_RENDER_NODE_CLASS(o) ((CstRenderNodeClass *)sys_class_cast_check(o, CST_TYPE_RENDER_NODE))
#define CST_RENDER_NODE_GET_CLASS(o) sys_instance_get_class(o, CstRenderNodeClass)

#define CST_RENDER_NODE_RCTX(o) cst_render_node_get_rctx(o)

struct _CstRenderNode {
  CstLayoutNode parent;

  /* < private > */
  CstNode *node;

  /* FRAWatch */
  SysList *awatch_list;

  /* CstNodeMap */
  SysList *nodemap_list;

  /* CstCssGroup */
  SysPtrArray* v_css_list;

  /* CST_NODE_LAYER_ENUM */
  CstLayerNode *layer_node;

  /* render context */
  CstRenderContext *rctx;
};

struct _CstRenderNodeClass {
  CstLayoutNodeClass parent;

  void (*construct) (CstRenderNode *self, CstNode* node);
  void (*relayout) (CstRenderNode* self, CstLayout* layout);
  void (*repaint) (CstRenderNode* self, CstLayout* layout);
};

SysType cst_render_node_get_type(void);
CstRenderNode *cst_render_node_new(void);
CstRenderNode *cst_render_node_new_I(CstNode *node);
void cst_render_node_construct (CstRenderNode *self, CstNode* node);

void cst_render_node_set_meta(const SysChar* name, SysType stype);
SysType cst_render_node_get_meta(const SysChar* name);

void cst_render_node_setup(void);
void cst_render_node_teardown(void);

#define cst_render_node_set_wrap(o, v) cst_render_context_set_wrap(CST_RENDER_NODE_RCTX(o), v)
#define cst_render_node_get_is_visible(o) cst_render_context_get_is_visible(CST_RENDER_NODE_RCTX(o))
#define cst_render_node_need_layout(o) cst_render_context_need_layout(CST_RENDER_NODE_RCTX(o))

/* css */
void cst_render_node_render_enter(CstRenderNode *self, CstLayout *layout);
void cst_render_node_render_leave(CstRenderNode *self, CstLayout *layout);

void cst_render_node_prepare(CstRenderNode * self, CstLayout * layout);
void cst_render_node_print(CstRenderNode * self, CstRenderNode * prnode);
SysType cst_render_node_get_node_type(CstRenderNode *self);

void cst_render_node_change_to_layer(CstRenderNode *self, CstLayer *tolayer);

void cst_render_node_ref_awatch(CstRenderNode *self, FRAWatch *o);
SysList * cst_render_node_get_awatch_list(CstRenderNode *self);

void cst_render_node_ref_nodemap(CstRenderNode *self, CstNodeMap *o);
SysList * cst_render_node_get_nodemap_list(CstRenderNode *self);

void cst_render_node_add_v_css(CstRenderNode *self, CstCssGroup* g);
SysPtrArray* cst_render_node_get_v_css_list(CstRenderNode *self);

void cst_render_node_set_name(CstRenderNode *self, const SysChar* name);
const SysChar* cst_render_node_get_name(CstRenderNode *self);

void cst_render_node_set_id(CstRenderNode *self, const SysChar* id);
const SysChar* cst_render_node_get_id(CstRenderNode *self);

void cst_render_node_set_layer_node(CstRenderNode *self, CstLayerNode * layer_node);
CstLayerNode * cst_render_node_get_layer_node(CstRenderNode *self);

CstRenderContext* cst_render_node_get_rctx(CstRenderNode *self);

SYS_END_DECLS

#endif

