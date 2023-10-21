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

SysBool cst_layout_is_state(CstLayout *self, SysInt state) {
  sys_return_val_if_fail(self, false);

  return self->state & state;
}

CstLayout *cst_layout_new_I(FRDraw *draw, FRRegion *region) {
  CstLayout *o = cst_layout_new();

  cst_layout_construct(o, draw, region);

  return o;
}

FRRegion *cst_layout_get_region(CstLayout* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->region;
}

FRDraw *cst_layout_get_draw(CstLayout* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->draw;
}

void cst_layout_get_buffer_size(CstLayout* self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);

  fr_draw_get_size(self->draw, width, height);
}

void cst_layout_set_mode(CstLayout *self, SysInt mode) {
  sys_return_if_fail(self != NULL);

  self->mode = mode;
}

SysInt cst_layout_get_mode(CstLayout *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->mode;
}

void cst_layout_set_stage(CstLayout *self, SysInt stage) {
  sys_return_if_fail(self != NULL);

  self->stage = stage;
}

SysInt cst_layout_get_stage(CstLayout *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->stage;
}

void cst_layout_set_state(CstLayout *self, SysInt state) {
  sys_return_if_fail(self != NULL);

  self->state = state;
}

SysInt cst_layout_get_state(CstLayout *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->state;
}

void cst_layout_set_layer(CstLayout *self, SysInt layer) {
  sys_return_if_fail(self != NULL);

  self->layer = layer;
}

SysInt cst_layout_get_layer(CstLayout *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->layer;
}

void cst_layout_begin_layout(CstLayout* self, SysInt layer) {
  self->state = CST_RENDER_STATE_LAYOUT;
  self->stage = CST_RENDER_STAGE_FIRST;
  self->mode = CST_RENDER_MODE_OUT_TO_IN;
  self->layer = layer;

  fr_draw_frame_begin(self->draw, self->region);
}

void cst_layout_end_layout(CstLayout* self) {

  fr_draw_frame_end(self->draw, self->region);
}

void cst_layout_begin_node(CstLayout* self) {

}

void cst_layout_end_node(CstLayout* self) {

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
