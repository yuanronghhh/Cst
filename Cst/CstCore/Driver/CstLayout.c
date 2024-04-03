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

CstSurface* cst_layout_get_default_surface(CstLayout* self) {
  CstSurface* surface;
  SysUInt len;

  len = self->surfaces.len;
  if (len == 0) { return NULL; }
  surface = self->surfaces.pdata[len - 1];

  return surface;
}

CstSurface* cst_layout_get_surface(CstLayout* self, SysUInt surf_idx) {
  sys_return_val_if_fail(self != NULL, NULL);

  if (surf_idx >= self->surfaces.len) {
    sys_warning_N("surface index not correct: %d", surf_idx);
    return NULL;
  }

  return self->surfaces.pdata[surf_idx];
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

void cst_layout_layout_root(CstLayout* self, CstRenderNode* rnode) {
  SysInt width = 0, height = 0;
  FrDrawContext* draw_context = self->draw_context;
  FrRegion* region = self->region;

  fr_draw_context_get_buffer_size(draw_context, &width, &height);
  init_body(rnode, width, height);
  fr_draw_context_set_surfaces(draw_context, &self->surfaces);

  fr_draw_context_frame_begin(draw_context, region);
  layout_node_r(rnode);
  fr_draw_context_frame_end(draw_context, region);
}

void cst_layout_paint_root(CstLayout* self, CstRenderNode* rnode) {
}

/* object api */
static void cst_layout_init(CstLayout *self) {
  CstSurface* paint_surface = cst_surface_create_image_surface(800, 600);
  
  sys_harray_init_with_free_func(&self->surfaces, _sys_object_unref);
  sys_harray_add(&self->surfaces, paint_surface);
}

static void cst_layout_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);
  CstLayout* self = CST_LAYOUT(o);

  sys_clear_pointer(&self->draw_context, _sys_object_unref);
  sys_harray_destroy(&self->surfaces);
  fr_region_destroy(self->region);

  SYS_OBJECT_CLASS(cst_layout_parent_class)->dispose(o);
}

static void cst_layout_class_init(CstLayoutClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layout_dispose;
}
