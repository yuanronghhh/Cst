#include <CstCore/Driver/CstBoxLayer.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Front/Common/CstNode.h>


struct _CstBoxLayerPrivate {
  /**
   * base layer, every node place here.
   */
  CstNode *tree;

  SysList *gap_nodes;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstBoxLayer, cst_box_layer, CST_TYPE_LAYER);


CstNode *cst_box_layer_get_root(CstBoxLayer *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstBoxLayerPrivate *priv = self->priv;

  return priv->tree;
}

void cst_box_layer_set_root(CstBoxLayer *self, CstNode *root) {
  sys_return_if_fail(self != NULL);

  CstBoxLayerPrivate *priv = self->priv;

  priv->tree = root;
}

void cst_box_layer_insert_after(CstBoxLayer *box, CstNode *parent, CstNode *last_child, CstNode *nnode) {
  sys_return_if_fail(box != NULL);
  sys_return_if_fail(parent != NULL);
  sys_return_if_fail(nnode != NULL);

  cst_node_insert_after(parent, last_child, nnode);
}

void cst_box_layer_append(CstLayer *layer, CstNode *parent, CstNode *child) {
  sys_return_if_fail(layer != NULL);
  sys_return_if_fail(child != NULL);

  cst_node_append(parent, child);

  sys_object_ref(child);
}

void box_layer_mark(CstNode *v_parent, CstNode *v_node, CstRender *v_render, FRRect *bound) {
  sys_return_if_fail(v_node != NULL);

  CstNode *v_children = cst_node_children(v_node);
  CstNode *v_next = cst_node_next(v_node);
  const FRRect *nbound = cst_node_get_bound(v_node);

  if (v_children) {
    box_layer_mark(v_node, v_children, v_render, bound);
  }

  if (!cst_node_layer_is(v_node, CST_LAYER_ABS)) {
    return;
  }

  if (!cst_node_is_visible(v_node)) {
    return;
  }

  if (fr_rect_is_overlap(bound, nbound)) {
    cst_node_set_is_dirty(v_node, true);
  }

  if (v_next) {
    box_layer_mark(v_parent, v_next, v_render, bound);
  }
}

void cst_box_layer_check_i(CstLayer *layer, CstRender *v_render, FRRect *bound) {
  CstNode *v_node;

  CstBoxLayer *self = CST_BOX_LAYER(layer);
  CstBoxLayerPrivate *priv = self->priv;

  sys_return_if_fail(priv->tree != NULL);

  v_node = priv->tree;

  box_layer_mark(NULL, v_node, v_render, bound);
}

static void cst_box_layer_render_i(CstLayer *layer, CstModule *v_module, CstRender *v_render) {
  sys_return_if_fail(layer != NULL);

  CstNode *v_node;
  FRRegion *region;
  const FRRect *rect;
  CstBoxLayer *self = CST_BOX_LAYER(layer);
  CstBoxLayerPrivate *priv = self->priv;

  sys_return_if_fail(priv->tree != NULL);

  v_node = priv->tree;

  rect = cst_node_get_bound(v_node);
  region = cst_render_create_region(v_render, rect);

  cst_render_frame_begin(v_render, region);

  cst_node_relayout_root(v_module, v_node, v_render);
  cst_node_repaint_root(v_module, v_node, v_render);

  cst_render_frame_end(v_render, region);

  fr_region_destroy(region);
}

void cst_box_layer_print_tree(CstBoxLayer *self) {

  CstBoxLayerPrivate *priv = self->priv;

  cst_node_print_node(priv->tree);
}

/* object api */
CstLayer* cst_box_layer_new(void) {
  return sys_object_new(CST_TYPE_BOX_LAYER, NULL);
}

static void cst_box_layer_dispose(SysObject* o) {
  CstBoxLayer *self = CST_BOX_LAYER(o);

  CstBoxLayerPrivate *priv = self->priv;
  CstNode *root = priv->tree;

  sys_list_free_full(priv->gap_nodes, (SysDestroyFunc)_sys_object_unref);

  if (root) {

    cst_node_unlink_node_r(root);
  }

  SYS_OBJECT_CLASS(cst_box_layer_parent_class)->dispose(o);
}

void cst_box_layer_construct(SysObject* o) {
  CstBoxLayer *self = CST_BOX_LAYER(o);

  SYS_OBJECT_CLASS(cst_box_layer_parent_class)->construct(o);

  CstBoxLayerPrivate *priv = self->priv;

  priv->tree = NULL;
  priv->gap_nodes = NULL;
}

CstLayer *cst_box_layer_new_I(void) {
  CstLayer *o = cst_box_layer_new();

  cst_box_layer_construct(SYS_OBJECT(o));

  return o;
}

static void cst_box_layer_class_init(CstBoxLayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstLayerClass *lcls = CST_LAYER_CLASS(cls);

  ocls->construct = (SysObjectFunc)cst_box_layer_construct;
  ocls->dispose = cst_box_layer_dispose;

  lcls->check = cst_box_layer_check_i;
  lcls->render = cst_box_layer_render_i;
}

static void cst_box_layer_init(CstBoxLayer *self) {
  self->priv = cst_box_layer_get_private(self);
}

