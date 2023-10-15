#include <CstCore/Front/Common/CstLContentContext.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstRenderNode.h>


SYS_DEFINE_TYPE(CstLContentContext, cst_lcontent_context, CST_TYPE_LAYOUT_CONTEXT);


/* sys object api */
CstRenderContext* cst_lcontent_context_new(void) {
  return sys_object_new(CST_TYPE_LCONTENT_CONTEXT, NULL);
}

CstRenderContext* cst_lcontent_context_new_I(void) {
  CstRenderContext* o = cst_lcontent_context_new();
  return o;
}

static void cst_lcontent_context_init(CstLContentContext *o) {
}

static void cst_lcontent_context_layout_self_i(CstRenderContext *self, CstRenderNode* rnode, CstLayout *layout) {
  SysInt w, h;

  // set self size
  cst_render_context_get_prefer_size(self, &w, &h);
  cst_render_node_set_size(rnode, w, h);

  CST_RENDER_CONTEXT_CLASS(cst_lcontent_context_parent_class)->layout_self(self, rnode, layout);
}

static void cst_lcontent_context_layout_children_i(CstRenderContext* self, CstRenderNode* render_node, CstLayout* layout) {
  cst_render_node_fill_rectangle(render_node, layout);

  CST_RENDER_CONTEXT_CLASS(cst_lcontent_context_parent_class)->layout_children(self, render_node, layout);
}

static void cst_lcontent_context_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lcontent_context_parent_class)->dispose(o);
}

static void cst_lcontent_context_class_init(CstLContentContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstRenderContextClass *ncls = CST_RENDER_CONTEXT_CLASS(cls);

  ocls->dispose = cst_lcontent_context_dispose;

  ncls->layout_self = cst_lcontent_context_layout_self_i;
  ncls->layout_children = cst_lcontent_context_layout_children_i;
}
