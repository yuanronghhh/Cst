#include <CstCore/Driver/CstLayer.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Front/Common/CstNode.h>

SYS_DEFINE_TYPE(CstLayer, cst_layer, SYS_TYPE_OBJECT);

void cst_layer_check(CstLayer *self, FRDraw *draw, FRRegion *region) {
  sys_return_if_fail(self != NULL);

  CstLayerClass *cls = CST_LAYER_GET_CLASS(self);
  sys_return_if_fail(cls->check != NULL);

  cls->check(self, draw, region);
}

void cst_layer_render(CstLayer *self, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstLayerClass *cls = CST_LAYER_GET_CLASS(self);
  sys_return_if_fail(cls->render != NULL);

  cls->render(self, draw, layout);
}

void cst_layer_rerender(CstLayer *self, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstLayerClass *cls = CST_LAYER_GET_CLASS(self);

  sys_return_if_fail(cls->render != NULL);

  cls->rerender(self, draw, layout);
}

void cst_layer_queue_draw_node(CstLayer *self, CstNode *v_node) {
  sys_return_if_fail(self != NULL);

  sys_object_ref(v_node);
  sys_queue_push_tail(self->draw_queue, v_node);
}

void cst_layer_rerender_i(CstLayer *self, FRDraw *draw, CstLayout *layout) {
}

void cst_layer_check_i(CstLayer *self, FRDraw *draw, FRRegion *region) {
}

void cst_layer_render_i(CstLayer *self, FRDraw *draw, CstLayout *layout) {
}

/* object api */
static void cst_layer_dispose(SysObject* o) {
  CstLayer *self = CST_LAYER(o);

  sys_queue_free_full(self->draw_queue, (SysDestroyFunc)_sys_object_unref);

  SYS_OBJECT_CLASS(cst_layer_parent_class)->dispose(o);
}

static void cst_layer_construct(CstLayer* self) {

  self->draw_queue = sys_queue_new();
}

CstLayer *cst_layer_new(void) {
  return sys_object_new(CST_TYPE_LAYER, NULL);
}

static void cst_layer_class_init(CstLayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layer_dispose;

  cls->construct = cst_layer_construct;
  cls->check = cst_layer_check_i;
  cls->rerender = cst_layer_rerender_i;
  cls->render = cst_layer_render_i;
}

static void cst_layer_init(CstLayer *self) {
}
