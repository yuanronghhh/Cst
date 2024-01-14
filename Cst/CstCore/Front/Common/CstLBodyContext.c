#include <CstCore/Front/Common/CstLBodyContext.h>

#include <CstCore/Driver/CstLayerNode.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstLayout.h>


SYS_DEFINE_TYPE(CstLBodyContext, cst_lbody_context, CST_TYPE_RENDER_CONTEXT);


static void cst_lbody_layout_self_i (CstRenderContext *o, CstRenderNode *rnode, CstLayout *layout) {
  SysInt width, height;

  cst_layout_get_buffer_size(layout, &width, &height);
  cst_render_node_set_size(rnode, width, height);

  o->prefer_width = width;
  o->prefer_height = height;

  cst_render_node_set_size(rnode, width, height);
}

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

static void cst_lbody_context_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lbody_context_parent_class)->dispose(o);
}

static void cst_lbody_context_class_init(CstLBodyContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstRenderContextClass *rcls = CST_RENDER_CONTEXT_CLASS(cls);

  ocls->dispose = cst_lbody_context_dispose;
  rcls->layout_self = cst_lbody_layout_self_i;
}
