#include <CstCore/Driver/CstBoxLayer.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstLayout.h>


SYS_DEFINE_TYPE(CstBoxLayer, cst_box_layer, CST_TYPE_LAYER);


CstBoxNode *cst_box_layer_get_root(CstBoxLayer *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->tree;
}

void cst_box_layer_set_root(CstBoxLayer *self, CstBoxNode *root) {
  sys_return_if_fail(self != NULL);

  self->tree = root;
}

void cst_box_layer_insert_after(CstBoxLayer *box, CstBoxNode *parent, CstBoxNode *last_child, CstBoxNode *nnode) {
  sys_return_if_fail(box != NULL);
  sys_return_if_fail(parent != NULL);
  sys_return_if_fail(nnode != NULL);

  cst_box_node_insert_after(parent, last_child, nnode);
}

void cst_box_layer_append(CstLayer *layer, CstBoxNode *parent, CstBoxNode *child) {
  sys_return_if_fail(layer != NULL);
  sys_return_if_fail(child != NULL);

  cst_box_node_append(parent, child);

  sys_object_ref(child);
}

SysInt box_node_check_dirty(CstBoxNode *box_node, FRRegion *region) {
  CstRenderNode *render_node = CST_RENDER_NODE(box_node);
  const FRRect *nbound = cst_render_node_get_bound (render_node);

  if(fr_region_is_empty(region)) {
    return -4;
  }

  if (cst_render_node_is_dirty(render_node)) {
    return -1;
  }

  if (!cst_render_node_is_visible(render_node)) {
    return -1;
  }

  SysInt s = fr_region_contains_rectangle(region, nbound);
  if (s == CAIRO_REGION_OVERLAP_OUT) {
    return -3;
  }

  return 1;
}

void bfs_box_layer_mark(CstBoxLayer *self, CstBoxNode *box_node, FRRegion *region) {
  sys_return_if_fail(box_node != NULL);

  CstBoxNode *nnode;
  CstRenderNode *render_node;
  CstBoxNode *nchild;
  SysInt status;
  CstLayer *layer = CST_LAYER(self);

  SysQueue *nqueue = sys_queue_new();

  sys_queue_push_head(nqueue, box_node);

  while (sys_queue_get_length(nqueue) > 0) {
    nnode = sys_queue_pop_head(nqueue);
    render_node = CST_RENDER_NODE(nnode);

    status = box_node_check_dirty(box_node, region);
    if (status < 0) {
      continue;
    }
    cst_render_node_set_paint(render_node, true);
    cst_layer_queue_draw_node(layer, render_node);

    nchild = cst_box_node_children(nnode);
    while (nchild) {
      sys_queue_push_tail(nqueue, nchild);

      nchild = cst_box_node_next(nchild);
    }
  }

  sys_queue_free(nqueue);
}

void cst_box_layer_check(CstBoxLayer *self, CstLayout *layout) {

  sys_return_if_fail(self->tree != NULL);
  CstBoxNode *box_node = self->tree;
  FRRegion *region = cst_layout_get_region(layout);

  bfs_box_layer_mark(self, box_node, region);
}

void cst_box_layer_render(CstBoxLayer *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->tree != NULL);

  CstBoxNode *box_node = self->tree;

  cst_box_node_repaint_root(box_node, layout);
}

void cst_box_layer_print_tree(CstBoxLayer *self) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->tree != NULL);

  cst_box_node_print_node_r(self->tree);
}

void cst_box_layer_layout(CstBoxLayer *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->tree != NULL);

  CstBoxNode *box_node = self->tree;

  cst_box_node_relayout_root(box_node, layout);
}

void cst_box_node_unlink_node_r(CstBoxNode *self) {
  sys_return_if_fail(self != NULL);

  SysObject *o = SYS_OBJECT(self);

  if(self->children) {
    cst_box_node_unlink_node_r(self->children);
  }

  if(self->next) {
    cst_box_node_unlink_node_r(self->next);
  }

  sys_object_unref(o);
}


/* object api */
CstLayer* cst_box_layer_new(void) {
  return sys_object_new(CST_TYPE_BOX_LAYER, NULL);
}

static void cst_box_layer_dispose(SysObject* o) {
  CstBoxLayer *self = CST_BOX_LAYER(o);

  CstBoxNode *root = self->tree;

  sys_list_free_full(self->gap_nodes, (SysDestroyFunc)_sys_object_unref);

  if (root) {

    cst_box_node_unlink_node_r(root);
  }

  SYS_OBJECT_CLASS(cst_box_layer_parent_class)->dispose(o);
}

static void cst_box_layer_construct(CstLayer* o) {
  CstBoxLayer *self = CST_BOX_LAYER(o);

  self->tree = NULL;
  self->gap_nodes = NULL;
}

CstLayer *cst_box_layer_new_I(void) {
  CstLayer *o = cst_box_layer_new();

  cst_box_layer_construct(o);

  return o;
}

static void cst_box_layer_class_init(CstBoxLayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstLayerClass *lcls = CST_LAYER_CLASS(cls);

  ocls->dispose = cst_box_layer_dispose;

  lcls->construct = cst_box_layer_construct;
}

static void cst_box_layer_init(CstBoxLayer *self) {
}

