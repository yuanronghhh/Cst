#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstAlgorithm.h>
#include <CstCore/Driver/CstSurface.h>


SYS_DEFINE_TYPE(CstLayout, cst_layout, SYS_TYPE_OBJECT);

CstLayout* cst_layout_new(void) {

  return sys_object_new(CST_TYPE_LAYOUT, NULL);
}

void cst_layout_set_region(CstLayout *self, FrRegion * region) {
  sys_return_if_fail(self != NULL);

  self->region = region;
}

FrRegion * cst_layout_get_region(CstLayout *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->region;
}

void cst_layout_set_state(CstLayout *self, SysInt state) {
  sys_return_if_fail(self != NULL);

  self->state = state;
}

SysInt cst_layout_get_state(CstLayout *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->state;
}

static void cst_layout_construct(CstLayout* self,
    FrDrawContext *draw_context,
    FrRegion *region) {
  self->state = 0;
  self->region = region;
  self->draw_context = sys_object_ref(draw_context);
}

CstLayout *cst_layout_new_I(FrDrawContext *draw_context, FrRegion *region) {
  CstLayout *o = cst_layout_new();

  cst_layout_construct(o, draw_context, region);

  return o;
}

static void init_body(CstRenderNode* rnode, SysInt width, SysInt height) {
  cst_render_node_set_prefer_size(rnode, width, height);
  cst_render_node_set_size(rnode, width, height);
}

void layout_node_r(CstRenderNode* rnode) {
  CstAlgorithm* alg;
  CstRenderContext* rctx;

  alg = cst_render_node_get_algorithm(rnode);
  rctx = cst_render_node_get_render_context(rnode);

  cst_algorithm_measure(alg, rnode, rctx);
  cst_algorithm_layout(alg, rnode, rctx);
}

void cst_layout_layout_prepare(CstLayout* self, CstRenderNode* rnode) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(rnode != NULL);

  FrDrawContext* draw_context = self->draw_context;
  SysInt width = 0, height = 0;

  fr_draw_context_get_buffer_size(draw_context, &width, &height);
  init_body(rnode, width, height);
}

void cst_layout_layout_begin(CstLayout* self) {
  sys_return_if_fail(self != NULL);

  fr_draw_context_frame_begin(self->draw_context, self->region);
}

void cst_layout_layout_end(CstLayout* self) {

  fr_draw_context_frame_end(self->draw_context, self->region);
}

void cst_layout_layout_root(CstLayout* self, CstRenderNode* rnode) {
  layout_node_r(rnode);
}

void cst_layout_paint_root(CstLayout* self, CstRenderNode* rnode) {
  CstSurface* ns = cst_layout_get_default_surface(self);
  FrSurface *surface =  FR_SURFACE(ns);

  FrContext* cr = fr_context_new_I(surface);
  const FrBound* bound = cst_render_node_get_bound(rnode);
  const FrSInt4* m4 = cst_render_node_get_margin(rnode);
  const FrSInt4* p4 = cst_render_node_get_padding(rnode);
  FrColor color = {1.0, 0.0, 0.0, 1.0};

  fr_draw_set_color(cr, &color);
  fr_context_stroke_mp(cr, bound, m4, p4);

  sys_object_unref(cr);
}

/* object api */
static void cst_layout_init(CstLayout *self) {
}

static void cst_layout_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);
  CstLayout* self = CST_LAYOUT(o);

  sys_clear_pointer(&self->region, fr_region_destroy);
  sys_clear_pointer(&self->draw_context, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_layout_parent_class)->dispose(o);
}

static void cst_layout_class_init(CstLayoutClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layout_dispose;
}
