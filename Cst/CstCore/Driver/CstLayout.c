#include <CstCore/Driver/CstLayout.h>

#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstRender.h>


SYS_DEFINE_TYPE(CstLayout, cst_layout, SYS_TYPE_OBJECT);

CstLayout* cst_layout_new(void) {

  return sys_object_new(CST_TYPE_LAYOUT, NULL);
}

static void cst_layout_construct(CstLayout* self, FrDraw *draw, FrRegion *region) {
  self->state = 0;
  self->region = region;
  self->draw = draw;
}

SysBool cst_layout_is_state(CstLayout *self, SysInt state) {
  sys_return_val_if_fail(self, false);

  return self->state & state;
}

CstLayout *cst_layout_new_I(FrDraw *draw, FrRegion *region) {
  CstLayout *o = cst_layout_new();

  cst_layout_construct(o, draw, region);

  return o;
}

FrRegion *cst_layout_get_region(CstLayout* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->region;
}

void cst_layout_get_size(CstLayout* self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->draw != NULL);

  fr_draw_get_buffer_size(self->draw, width, height);
}

void cst_layout_set_state(CstLayout *self, CST_RENDER_STATE_ENUM state) {
  sys_return_if_fail(self != NULL);

  self->state = state;
}

CST_RENDER_STATE_ENUM cst_layout_get_state(CstLayout *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->state;
}

void cst_layout_begin_layout(CstLayout* self) {
  self->state = CST_RENDER_STATE_LAYOUT;

  fr_draw_frame_begin(self->draw, self->region);
}

void cst_layout_end_layout(CstLayout* self) {

  fr_draw_frame_end(self->draw, self->region);
}

/* object api */
static void cst_layout_init(CstLayout *self) {
}

static void cst_layout_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);
  CstLayout* self = CST_LAYOUT(o);

  sys_clear_pointer(&self->draw, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_layout_parent_class)->dispose(o);
}

static void cst_layout_class_init(CstLayoutClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layout_dispose;
}
