#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstLayoutContext.h>


SYS_DEFINE_TYPE(CstLayout, cst_layout, SYS_TYPE_OBJECT);

CstLayout* cst_layout_new(void) {
  return sys_object_new(CST_TYPE_LAYOUT, NULL);
}

static void cst_layout_construct(CstLayout* self) {

  self->state = 0;
}

SysBool cst_layout_state_layout(CstLayout *self) {
  sys_return_val_if_fail(self, false);

  return self->state & CST_RENDER_STATE_LAYOUT;
}

CstLayout *cst_layout_new_I(void) {
  CstLayout *o = cst_layout_new();

  cst_layout_construct(o);

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

FRContext *cst_layout_get_cr(CstLayout *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->cr;
}

void cst_layout_layout(CstLayout* self, CstLayoutNode* p_layout_node, CstLayoutNode* lnode, CstLayoutContext *lctx) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(p_layout_node != NULL);

  if (!cst_layout_context_need_layout(lctx)) {
    return;
  }

  cst_paint_context_paint(self, );
  cst_node_render_enter(self, cr, layout);
  cst_node_layout_init(self);

  if (self->children) {
    cst_node_layout(v_module, self, self->children, draw, layout);
  }

  cst_node_expand(self);

  cst_node_relayout(v_module, p_layout_node, self, draw, layout);

  if (self->next) {
    cst_node_layout(v_module, p_layout_node, self->next, draw, layout);
  }

  cst_node_render_leave(self, cr, layout);
  cst_node_set_need_relayout(self, false);
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
  cls->construct = cst_layout_construct;
}
