#include <CstCore/Front/Common/CstLBody.h>

#include <CstCore/Driver/CstNodeBuilder.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Front/Common/CstLBodyContext.h>


SYS_DEFINE_TYPE(CstLBody, cst_lbody, CST_TYPE_RENDER_NODE);


static void cst_lbody_layout_i(CstLayoutNode *lnode, CstLayout* layout) {
  SysInt w, h;

  cst_layout_get_buffer_size(layout, &w, &h);
  cst_layout_node_set_size(lnode, w, h);
}

static SysObject* cst_lbody_dclone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  SysObject *n = SYS_OBJECT_CLASS(cst_lbody_parent_class)->dclone(o);

  return n;
}

CstRenderNode* cst_lbody_new(void) {
  return sys_object_new(CST_TYPE_LBODY, NULL);
}

void cst_lbody_construct(CstRenderNode *rnode, CstNode *node) {
  sys_return_if_fail(rnode != NULL);

  CstNodeBuilder *builder = cst_node_get_builder(node);
  sys_return_if_fail(builder != NULL);

  cst_node_builder_set_id(builder, "id.body.0");
  cst_node_builder_set_position(builder, CST_NODE_POSITION_BOX);

  CST_RENDER_NODE_CLASS(cst_lbody_parent_class)->construct(rnode, node);
}

static void cst_lbody_init(CstLBody *self) {
}

static void cst_lbody_class_init(CstLBodyClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstRenderNodeClass *ncls = CST_RENDER_NODE_CLASS(cls);
  CstLayoutNodeClass* lcls = CST_LAYOUT_NODE_CLASS(cls);

  ocls->dispose = cst_lbody_dispose;
  ocls->dclone = cst_lbody_dclone_i;

  ncls->construct = cst_lbody_construct;
  lcls->layout = cst_lbody_layout_i;
}

static void cst_lbody_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lbody_parent_class)->dispose(o);
}
