#include <CstCore/Driver/CstBoxLayer.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstRenderContext.h>

typedef struct _BoxLayerContext BoxLayerContext;

struct _BoxLayerContext {
  CstLayer* v_layer;
  FRRegion* v_region;
};

SYS_DEFINE_TYPE(CstBoxLayer, cst_box_layer, CST_TYPE_LAYER);


CstBoxNode *cst_box_layer_get_root(CstBoxLayer *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->tree;
}

void cst_box_layer_set_root(CstBoxLayer *self, CstBoxNode *root) {
  sys_return_if_fail(self != NULL);

  self->tree = root;
}

static void box_layer_mark_one(CstRenderNode* rnode, BoxLayerContext *ctx) {
  CstLayer* self = ctx->v_layer;
  FRRegion* region = ctx->v_region;
  CstRenderContext *rctx = cst_render_node_get_render_ctx(rnode);
  CstNode *node = cst_render_node_get_node(rnode);
  const FRRect *bound = cst_layout_node_get_bound(CST_LAYOUT_NODE(node));

  sys_return_if_fail(region != NULL);
  sys_return_if_fail(self != NULL);

  if (fr_region_is_empty(region)) {
    return;
  }

  if (cst_render_context_is_dirty(rctx)) {
    return;
  }

  if (!cst_render_context_get_is_visible(rctx)) {
    return;
  }

  SysUInt s = fr_region_contains_rectangle(region, bound);
  if (s == FR_REGION_OVERLAP_OUT) {
    return;
  }

  sys_assert(bound->width != -1 && "width should be set before check dirty.");

  cst_render_context_set_paint(rctx, true);
  cst_layer_queue_draw_node(self, rnode);
}

void cst_box_layer_check(CstLayer *o, CstLayout *layout) {
  CstBoxLayer* self = CST_BOX_LAYER(o);
  sys_return_if_fail(self->tree != NULL);

  FRRegion *region = cst_layout_get_region(layout);
  BoxLayerContext ctx = { o, region };

  cst_box_node_bfs_handle(self->tree, (CstRenderNodeFunc)box_layer_mark_one, &ctx);
}

void cst_box_layer_render(CstLayer*o, CstLayout *layout) {
  CstBoxLayer* self = CST_BOX_LAYER(o);
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->tree != NULL);

  CstBoxNode *box_node = self->tree;

  cst_box_node_repaint_root(box_node, layout);
}

void cst_box_layer_layout(CstLayer* o, CstLayout* layout) {
  CstBoxLayer* self = CST_BOX_LAYER(o);

  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->tree != NULL);

  CstBoxNode* box_node = self->tree;
  
  cst_box_node_relayout_node(box_node, layout);

  // cst_box_layer_print_tree(self);
}

void box_node_print(CstRenderNode* rnode, SysPointer user_data) {
  
  cst_render_node_print(rnode);
}

void cst_box_layer_print_tree(CstBoxLayer *self) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->tree != NULL);

  cst_box_node_bfs_handle(self->tree, box_node_print, NULL);
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

CstRenderNode* cst_box_layer_realize_node(CstBoxLayer *box_layer, CstBoxNode *parent, CstNode *node) {
  sys_return_val_if_fail(box_layer != NULL, NULL);
  sys_return_val_if_fail(node != NULL, NULL);

  CstRenderNode* child;

  parent = parent ? parent : box_layer->tree;

  child = cst_box_node_new_I(node);

  cst_box_node_append(parent, CST_BOX_NODE(child));

  return child;
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
  CST_LAYER_CLASS(cst_box_layer_parent_class)->construct(o);

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

