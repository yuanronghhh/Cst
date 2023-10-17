#include <CstCore/Front/Common/CstLGridContext.h>
#include <CstCore/Driver/CstRenderNode.h>


SYS_DEFINE_TYPE(CstLGridContext, cst_lgrid_context, CST_TYPE_LAYOUT_CONTEXT);

/* sys object api */
CstRenderContext* cst_lgrid_context_new(void) {
  return sys_object_new(CST_TYPE_LGRID_CONTEXT, NULL);
}

static void cst_lgrid_context_init(CstLGridContext *o) {
}

static void cst_lgrid_context_layout_self_i(CstRenderContext *self, CstRenderNode* render_node, CstLayout *layout) {
  SysInt w, h;

  CstLayoutNode *lnode = CST_LAYOUT_NODE(render_node);

  cst_render_context_get_prefer_size(self, &w, &h);
  cst_layout_node_set_size(lnode, w, h);

  CST_RENDER_CONTEXT_CLASS(cst_lgrid_context_parent_class)->layout_self(self, render_node, layout);
}

static void cst_lgrid_context_layout_children_i(CstRenderContext* self, CstRenderNode* render_node, CstLayout* layout) {
  CST_RENDER_CONTEXT_CLASS(cst_lgrid_context_parent_class)->layout_children(self, render_node, layout);
  CstNode* node = cst_render_node_get_node(render_node);

  cst_layout_node_fill_rectangle(CST_LAYOUT_NODE(node), layout);
}

static void cst_lgrid_context_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lgrid_context_parent_class)->dispose(o);
}

static void cst_lgrid_context_class_init(CstLGridContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstRenderContextClass *ncls = CST_RENDER_CONTEXT_CLASS(cls);

  ocls->dispose = cst_lgrid_context_dispose;

  ncls->layout_self = cst_lgrid_context_layout_self_i;
  ncls->layout_children = cst_lgrid_context_layout_children_i;
}
