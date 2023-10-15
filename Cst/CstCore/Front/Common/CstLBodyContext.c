#include <CstCore/Front/Common/CstLBodyContext.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstLayout.h>


SYS_DEFINE_TYPE(CstLBodyContext, cst_lbody_context, CST_TYPE_LAYOUT_CONTEXT);


/* sys object api */
CstRenderContext* cst_lbody_context_new(void) {
  return sys_object_new(CST_TYPE_LBODY_CONTEXT, NULL);
}

CstRenderContext* cst_lbody_context_new_I(void) {
  CstRenderContext* o = cst_lbody_context_new();

  return o;
}

static void cst_lbody_context_init(CstLBodyContext *o) {
}

static void cst_lbody_context_layout_self_i(CstRenderContext *self, CstRenderNode* render_node, CstLayout *layout) {
  SysInt w, h;

  CstLayoutNode *lnode = CST_LAYOUT_NODE(render_node);

  cst_layout_get_buffer_size(layout, &w, &h);
  cst_layout_node_set_size(lnode, w, h);
  cst_render_context_set_prefer_size(self, w, h);

  CST_RENDER_CONTEXT_CLASS(cst_lbody_context_parent_class)->layout_self(self, render_node, layout);
}

static void cst_lbody_context_layout_children_i(CstRenderContext* self, CstRenderNode* render_node, CstLayout* layout) {
  cst_render_node_fill_rectangle(render_node, layout);
  
  CST_RENDER_CONTEXT_CLASS(cst_lbody_context_parent_class)->layout_children(self, render_node, layout);
}

static void cst_lbody_context_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lbody_context_parent_class)->dispose(o);
}

static void cst_lbody_context_class_init(CstLBodyContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstRenderContextClass *ncls = CST_RENDER_CONTEXT_CLASS(cls);

  ocls->dispose = cst_lbody_context_dispose;

  ncls->layout_self = cst_lbody_context_layout_self_i;
  ncls->layout_children = cst_lbody_context_layout_children_i;
}
