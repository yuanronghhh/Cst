#include <CstCore/Driver/CstLayer.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Front/Common/CstNode.h>


struct _CstLayerPrivate {
  SysQueue *draw_queue;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstLayer, cst_layer, SYS_TYPE_OBJECT);

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

  CstLayerPrivate *priv = self->priv;

  sys_object_ref(v_node);
  sys_queue_push_tail(priv->draw_queue, v_node);
}

void cst_layer_rerender_i(CstLayer *self, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstNode *v_node;
  CstNode *v_parent;

  CstLayerPrivate *priv = self->priv;
  SysQueue *draw_queue = priv->draw_queue;

  for (SysList *item = draw_queue->head; item; item = item->next) {
    v_node = item->data;
    v_parent = cst_node_parent(v_node);

    if (v_parent == NULL) {
      cst_node_relayout_root(NULL, NULL, v_node, draw, layout);

    } else {
      cst_node_layout(NULL, v_parent, v_node, draw, layout);
    }
  }

  while(sys_queue_get_length(draw_queue) > 0) {
    v_node = sys_queue_pop_head(draw_queue);
    v_parent = cst_node_parent(v_node);

    cst_node_paint(NULL, v_parent, v_node, draw, layout);
    sys_object_unref(v_node);
  }
}

void cst_layer_check_i(CstLayer *layer, FRDraw *draw, FRRegion *region) {
}

void cst_layer_render_i(CstLayer *layer, FRDraw *v_draw, CstLayout *layout) {
}

/* object api */
static void cst_layer_dispose(SysObject* o) {
  CstLayer *self = CST_LAYER(o);
  CstLayerPrivate *priv = self->priv;

  sys_queue_free_full(priv->draw_queue, (SysDestroyFunc)_sys_object_unref);

  SYS_OBJECT_CLASS(cst_layer_parent_class)->dispose(o);
}

static void cst_layer_construct(CstLayer* self) {
  CstLayerPrivate *priv = self->priv;

  priv->draw_queue = sys_queue_new();
}

CstLayer *cst_layer_new(void) {
  return sys_object_new(CST_TYPE_LAYER, NULL);
}

static void cst_layer_class_init(CstLayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layer_dispose;

  cls->construct = cst_layer_construct;
  cls->rerender = cst_layer_rerender_i;
  cls->check = cst_layer_check_i;
  cls->render = cst_layer_render_i;
}

static void cst_layer_init(CstLayer *self) {
  self->priv = cst_layer_get_private(self);
}
