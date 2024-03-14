#include <CstCore/Driver/CstBoxLayer.h>

#include <CstCore/Front/Common/CstLBoxContext.h>
#include <CstCore/Driver/CstLayerNode.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstLayout.h>

typedef struct _BoxLayerPass BoxLayerPass;

struct _BoxLayerPass {
  CstLayer* v_layer;
  FRRegion* v_region;
};

SYS_DEFINE_TYPE(CstBoxLayer, cst_box_layer, CST_TYPE_LAYER);


static void cst_box_layer_set_root_i (CstLayer *o, CstLayerNode *root) {
  sys_return_if_fail(o != NULL);
  CstBoxLayer *self = CST_BOX_LAYER(o);
  CstBoxNode *bnode = CST_BOX_NODE(root);

  self->tree = bnode;
}

CstLayerNode* cst_box_layer_get_root_i(CstLayer* o) {
  sys_return_val_if_fail(o != NULL, NULL);
  CstBoxLayer *self = CST_BOX_LAYER(o);

  return CST_LAYER_NODE(self->tree);
}

static SysBool box_layer_mark_one(CstRenderNode* rnode, BoxLayerPass* ctx) {
  CstLayerNode* lnode;
  CstLayer* self;
  FRRegion* region;
  const FRRect* bound;

  self = ctx->v_layer;
  region = ctx->v_region;
  lnode = rnode->layer_node;
  bound = cst_render_node_get_bound(rnode);

  sys_return_val_if_fail(region != NULL, false);
  sys_return_val_if_fail(self != NULL, false);

  if (fr_region_is_empty(region)) {
    return false;
  }

  if (!cst_render_node_is_visible(rnode)) {
    return false;
  }

  if (cst_render_node_is_dirty(rnode)) {
    return false;
  }

  SysUInt s = fr_region_contains_rectangle(region, bound);
  if (s == FR_REGION_OVERLAP_OUT) {
    return false;
  }

  sys_assert(bound->width != -1 && "width should be set before check dirty.");

  cst_render_node_set_need_layout(rnode, true);
  cst_layer_queue_draw_node(self, lnode);

  return true;
}

static void cst_box_layer_check_i(CstLayer *o, CstLayout *layout) {
  CstBoxLayer* self = CST_BOX_LAYER(o);
  sys_return_if_fail(self->tree != NULL);

  FRRegion *region = cst_layout_get_region(layout);
  BoxLayerPass ctx = { o, region };

  cst_box_node_bfs_handle(self->tree, (CstBoxNodeFunc)box_layer_mark_one, &ctx);
}

void cst_box_layer_print_tree(CstBoxLayer *self) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->tree != NULL);

  cst_box_node_bfs_handle(self->tree, cst_box_node_print, NULL);
}

CstLayerNode *cst_box_layer_new_node_i(CstLayer *layer, CstLayerNode *parent) {
  CstLayerNode *lnode = cst_box_node_new_I();

  if (parent) {

    cst_box_node_append(CST_BOX_NODE(parent), CST_BOX_NODE(lnode));
  }

  return lnode;
}

/* object api */
CstLayer* cst_box_layer_new(void) {
  return sys_object_new(CST_TYPE_BOX_LAYER, NULL);
}

static SysBool box_node_unlink(CstBoxNode *bnode, CstBoxLayer *layer) {
  sys_object_unref(bnode);
  return true;
}

static void cst_box_layer_dispose(SysObject* o) {
  CstBoxLayer *self = CST_BOX_LAYER(o);

  CstBoxNode *root = CST_BOX_NODE(self->tree);

  sys_list_free_full(self->gap_nodes, (SysDestroyFunc)_sys_object_unref);

  if (root) {

    cst_box_node_handle_ft_r(root, (CstBoxNodeFunc)box_node_unlink, self);
  }

  SYS_OBJECT_CLASS(cst_box_layer_parent_class)->dispose(o);
}

CstLayer *cst_box_layer_new_I(void) {
  return cst_box_layer_new();
}

static void cst_box_layer_class_init(CstBoxLayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstLayerClass *lcls = CST_LAYER_CLASS(cls);

  ocls->dispose = cst_box_layer_dispose;

  lcls->new_node = cst_box_layer_new_node_i;
  lcls->check = cst_box_layer_check_i;
  lcls->get_root = cst_box_layer_get_root_i;
  lcls->set_root = cst_box_layer_set_root_i;
}

static void cst_box_layer_init(CstBoxLayer *self) {
  self->tree = NULL;
  self->gap_nodes = NULL;
  cst_layer_set_name(CST_LAYER(self), "BoxLayer");
}

