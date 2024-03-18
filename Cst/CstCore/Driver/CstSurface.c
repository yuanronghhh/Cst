#include <CstCore/Driver/CstSurface.h>

#include <CstCore/Driver/CstBoxLayer.h>
#include <CstCore/Driver/CstAbsLayer.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstAlgorithm.h>
#include <CstCore/Driver/CstRenderNode.h>


SYS_DEFINE_TYPE(CstSurface, cst_surface, SYS_TYPE_OBJECT);


void cst_surface_layout_r(CstSurface* self, CstRenderNode* rnode, CstLayout* layout) {
  CstAlgorithm* alg;

  cst_layout_set_layer(layout, self->box_layer);
  cst_layout_begin_layout(layout);

  alg = cst_render_node_get_algorithm(rnode);
  cst_algorithm_layout(alg, rnode, layout);

  cst_layout_end_layout(layout);

  cst_layout_set_layer(layout, self->abs_layer);
  cst_layout_begin_layout(layout);

  alg = cst_render_node_get_algorithm(rnode);
  cst_algorithm_layout(alg, rnode, layout);

  cst_layout_end_layout(layout);
}

CstLayer* cst_surface_get_layer_by_type(CstSurface* self, SysInt layer_type) {
  sys_return_val_if_fail(self != NULL, NULL);

  switch (layer_type) {
  case CST_NODE_LAYER_BOX:
    return self->box_layer;
  case CST_NODE_LAYER_ABS:
    return self->abs_layer;
  case CST_NODE_LAYER_PASS:
    sys_warning_N("layer not implement: %d", layer_type);
    break;
  default:
    break;
  }

  return NULL;
}

CstSurface* cst_surface_create_image_surface(SysInt width, SysInt height) {
  FRSurface *fsur = fr_draw_create_image_surface(width, height);
  CstSurface* sur = cst_surface_new_I(fsur);

  return sur;
}

CstSurface* cst_surface_create_by_window(FRWindow *window, SysInt width, SysInt height) {
  FRSurface* fsur = fr_draw_create_surface(window, width, height);
  CstSurface* sur = cst_surface_new_I(fsur);

  return sur;
}

/* object api */
static void cst_surface_construct(CstSurface *self, FRSurface* surface) {
  self->surface = surface;

  self->box_layer = cst_box_layer_new_I();
  self->abs_layer = cst_abs_layer_new_I();
}

CstSurface* cst_surface_new(void) {
  return sys_object_new(CST_TYPE_SURFACE, NULL);
}

CstSurface *cst_surface_new_I(FRSurface *surface) {
  CstSurface *o = cst_surface_new();

  cst_surface_construct(o, surface);

  return o;
}

static void cst_surface_dispose(SysObject* o) {
  CstSurface *self = CST_SURFACE(o);

  sys_clear_pointer(&self->box_layer, _sys_object_unref);
  sys_clear_pointer(&self->abs_layer, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_surface_parent_class)->dispose(o);
}

static void cst_surface_class_init(CstSurfaceClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_surface_dispose;
}

void cst_surface_init(CstSurface* self) {
}

