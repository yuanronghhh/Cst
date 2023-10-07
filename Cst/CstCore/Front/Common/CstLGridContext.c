#include <CstCore/Front/Common/CstLGridContext.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstRenderNode.h>


SYS_DEFINE_TYPE(CstLGridContext, cst_lgrid_context, CST_TYPE_LAYOUT_CONTEXT);

/* constraint */
void cst_constrain_same_width(CstRenderNode* self, CstLayoutContext* pctx) {
  CstLayoutContext* layout_ctx = cst_render_get_layout_context(self);
  CstLayoutNode* layout_node = cst_render_get_layout_node(self);

  layout_node->bound.height = pctx->prefer_width - layout_ctx->mbp.m3 - layout_ctx->mbp.m1;
}

void cst_constrain_same_height(CstRenderNode* self, CstLayoutContext* pctx) {
  CstLayoutContext* layout_ctx = cst_render_get_layout_context(self);
  CstLayoutNode* layout_node = cst_render_get_layout_node(self);

  layout_node->bound.height = pctx->prefer_height - layout_ctx->mbp.m0 - layout_ctx->mbp.m2;
}

/* sys object api */
CstLayoutContext* cst_lgrid_context_new(void) {
  return sys_object_new(CST_TYPE_LGRID_CONTEXT, NULL);
}

static void cst_lgrid_context_init(CstLGridContext *o) {
}

static void cst_lgrid_context_layout_self_i(CstLayoutContext *self, CstLayoutNode* layout_node, CstLayout *layout) {
  SysInt w, h;

  cst_layout_context_get_prefer_size(self, &w, &h);
  cst_layout_node_set_size(layout_node, w, h);

  CST_LAYOUT_CONTEXT_CLASS(cst_lgrid_context_parent_class)->layout_self(self, layout_node, layout);
}

static void cst_lgrid_context_layout_children_i(CstLayoutContext* self, CstRenderNode* render_node, CstLayout* layout) {
  CST_LAYOUT_CONTEXT_CLASS(cst_lgrid_context_parent_class)->layout_children(self, render_node, layout);

  cst_render_node_fill_rectangle(render_node, layout);
}

static void cst_lgrid_context_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lgrid_context_parent_class)->dispose(o);
}

static void cst_lgrid_context_class_init(CstLGridContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstLayoutContextClass *ncls = CST_LAYOUT_CONTEXT_CLASS(cls);

  ocls->dispose = cst_lgrid_context_dispose;

  ncls->layout_self = cst_lgrid_context_layout_self_i;
  ncls->layout_children = cst_lgrid_context_layout_children_i;
}
