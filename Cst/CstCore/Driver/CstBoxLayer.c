#include <CstCore/Driver/CstBoxLayer.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Front/Common/CstNode.h>

typedef struct _BFSLevel BFSLevel;

struct _BFSLevel {
  SysUInt level;
  SysQueue *lqueue;
};


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

SysInt box_node_mark_dirty(CstNode *v_node, FRRegion *region) {
  const FRRect *nbound = cst_node_get_bound(v_node);

  if(fr_region_is_empty(region)) {
    return -4;
  }

  if (cst_node_get_is_dirty(v_node)) {
    return -1;
  }

  if (!cst_node_is_visible(v_node)) {
    return -1;
  }

  if (cst_node_layer_has_flag(v_node, CST_LAYER_ABS)) {
    return -2;
  }

  SysInt s = fr_region_contains_rectangle(region, nbound);
  if (s == CAIRO_REGION_OVERLAP_OUT) {
    return -3;
  }

  cst_node_set_is_dirty(v_node, true);

  return 1;
}

/* bfs level */
static BFSLevel *bfs_level_new(SysUInt level) {
  BFSLevel *nlevel = sys_new0_N(BFSLevel, 1);

  nlevel->lqueue = sys_queue_new();
  nlevel->level = level;

  return nlevel;
}

static void bfs_level_push_head(BFSLevel *self, SysPointer ptr) {
  sys_queue_push_head(self->lqueue, ptr);
}

static SysUInt bfs_level_get_length(BFSLevel *self) {
  return sys_queue_get_length(self->lqueue);
}

static SYS_INLINE SysPointer bfs_level_pop_head(BFSLevel *self) {
  return sys_queue_pop_head(self->lqueue);
}

static void bfs_level_push_tail(BFSLevel *self, SysPointer ptr) {
  sys_queue_push_tail(self->lqueue, ptr);
}

static SysUInt bfs_level_get_level(BFSLevel *self) {
  return self->level;
}

static void bfs_level_free(BFSLevel *self) {
  sys_queue_free(self->lqueue);
  sys_free_N(self);
}

static void bfs_level_set_level(BFSLevel *self, SysUInt level) {
  self->level = level;
}

void bfs_box_layer_mark(CstLayer *layer, CstNode *v_node, FRRegion *region) {
  sys_return_if_fail(v_node != NULL);

  CstNode *nnode;
  CstNode *nchild;
  SysInt status;
  SysQueue *nqueue = sys_queue_new();

  sys_queue_push_head(nqueue, v_node);

  while (sys_queue_get_length(nqueue) > 0) {
    nnode = sys_queue_pop_head(nqueue);

    status = box_node_mark_dirty(nnode, region);
    if (status < 0) {
      continue;
    }

    cst_layer_queue_draw_node(layer, nnode);

    nchild = cst_node_children(nnode);
    while (nchild) {
      sys_queue_push_tail(nqueue, nchild);

      nchild = cst_node_next(nchild);
    }
  }

  sys_queue_free(nqueue);
}

void cst_box_layer_check_i(CstLayer *layer, FRDraw *draw, FRRegion *region) {
  CstNode *v_node;

  CstBoxLayer *self = CST_BOX_LAYER(layer);
  CstBoxLayerPrivate *priv = self->priv;

  sys_return_if_fail(priv->tree != NULL);

  v_node = priv->tree;

  bfs_box_layer_mark(layer, v_node, region);
}

static void cst_box_layer_render_i(CstLayer *layer, FRDraw *draw, FRContext *cr) {
  sys_return_if_fail(layer != NULL);

  CstNode *v_node;
  CstBoxLayer *self = CST_BOX_LAYER(layer);
  CstBoxLayerPrivate *priv = self->priv;

  sys_return_if_fail(priv->tree != NULL);

  v_node = priv->tree;

  cst_node_relayout_root(NULL, NULL, v_node, cr, draw, CST_RENDER_STATE_LAYOUT);
  cst_node_repaint_root(NULL, NULL, v_node, cr, draw, CST_RENDER_STATE_PAINT);
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

