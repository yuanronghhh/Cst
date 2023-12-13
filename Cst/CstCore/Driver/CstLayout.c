#include <CstCore/Driver/CstLayout.h>

#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstRender.h>


SYS_DEFINE_TYPE(CstLayout, cst_layout, SYS_TYPE_OBJECT);

CstLayout* cst_layout_new(void) {

  return sys_object_new(CST_TYPE_LAYOUT, NULL);
}

static void cst_layout_construct(CstLayout* self, CstRender *render, FRRegion *region) {
  FRWindow *window;

  self->state = 0;
  self->region = region;
  self->render = render;

  window = cst_render_get_default_window(render);
  self->draw = fr_draw_new_I(window);
}

SysBool cst_layout_is_state(CstLayout *self, SysInt state) {
  sys_return_val_if_fail(self, false);

  return self->state & state;
}

CstLayout *cst_layout_new_I(CstRender *render, FRRegion *region) {
  CstLayout *o = cst_layout_new();

  cst_layout_construct(o, render, region);

  return o;
}

FRRegion *cst_layout_get_region(CstLayout* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->region;
}

void cst_layout_set_draw(CstLayout *self, FRDraw * draw) {
  sys_return_if_fail(self != NULL);

  self->draw = draw;
}

FRDraw * cst_layout_get_draw(CstLayout *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->draw;
}

void cst_layout_get_buffer_size(CstLayout* self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);
  FRWindow *window = cst_render_get_default_window(self->render);

  fr_window_get_framebuffer_size(window, width, height);
}

void cst_layout_set_state(CstLayout *self, SysInt state) {
  sys_return_if_fail(self != NULL);

  self->state = state;
}

SysInt cst_layout_get_state(CstLayout *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->state;
}

void cst_layout_set_layer(CstLayout *self, CstLayer * layer) {
  sys_return_if_fail(self != NULL);

  self->layer = layer;
}

CstLayer * cst_layout_get_layer(CstLayout *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->layer;
}

void cst_layout_set_render(CstLayout *self, CstRender * render) {
  sys_return_if_fail(self != NULL);

  self->render = render;
}

CstRender * cst_layout_get_render(CstLayout *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->render;
}

void cst_layout_begin_layout(CstLayout* self, CstLayer *layer) {
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
  CstLayout* self = CST_LAYOUT(o);

  sys_clear_pointer(&self->draw, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_layout_parent_class)->dispose(o);
}

static void cst_layout_class_init(CstLayoutClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layout_dispose;
}
