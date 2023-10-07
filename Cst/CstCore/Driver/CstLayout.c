#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstRenderNode.h>


SYS_DEFINE_TYPE(CstLayout, cst_layout, SYS_TYPE_OBJECT);

CstLayout* cst_layout_new(void) {
  return sys_object_new(CST_TYPE_LAYOUT, NULL);
}

static void cst_layout_construct(CstLayout* self, FRDraw *draw, FRRegion *region) {
  self->state = 0;
  self->region = region;
  self->draw = draw;
}

SysBool cst_layout_state_layout(CstLayout *self) {
  sys_return_val_if_fail(self, false);

  return self->state & CST_RENDER_STATE_LAYOUT;
}

CstLayout *cst_layout_new_I(FRDraw *draw, FRRegion *region) {
  CstLayout *o = cst_layout_new();

  cst_layout_construct(o, draw, region);

  return o;
}

int cst_layout_get_state(CstLayout *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->state;
}

void cst_layout_set_state(CstLayout *self, int state) {
  sys_return_if_fail(self != NULL);

  self->state = state;
}

void cst_layout_layout_box(CstLayout* self, FRDraw *draw, CstRenderNode *render_node) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(render_node != NULL);

  // if (!cst_render_node_need_layout(render_node)) {
  //   return;
  // }
  // 
  // cst_render_node_prepare(render_node);
  // 
  // if (cst_render_node_children(self)) {
  //   cst_render_node_layout(render_node, self);
  // }
  // 
  // cst_node_expand(self);
  // 
  // cst_node_relayout(v_module, p_layout_node, self, draw, layout);
  // 
  // if (self->next) {
  //   cst_node_layout(v_module, p_layout_node, self->next, draw, layout);
  // }
  // 
  // cst_node_render_leave(self, cr, layout);
  // cst_node_set_need_relayout(self, false);
}

FRRegion *cst_layout_get_region(CstLayout* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->region;
}

FRDraw *cst_layout_get_draw(CstLayout* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->draw;
}

/* object api */
static void cst_layout_init(CstLayout *self) {
}

static void cst_layout_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  SYS_OBJECT_CLASS(cst_layout_parent_class)->dispose(o);
}

static void cst_layout_class_init(CstLayoutClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layout_dispose;
}
