#include <CstCore/Driver/CstAbsLayer.h>
#include <CstCore/Driver/CstILayer.h>
#include <CstCore/Driver/CstAbsNode.h>
#include <CstCore/Driver/CstLayerNode.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstLayout.h>

typedef struct _AbsLayerPass AbsLayerPass;

struct _AbsLayerPass {
  CstLayer* v_layer;
  FrRegion* v_region;
};

static void i_layer_imp(CstILayerInterface *iface);

SYS_DEFINE_WITH_CODE(CstAbsLayer, cst_abs_layer, CST_TYPE_LAYER,
    SYS_IMPLEMENT_INTERFACE(CST_TYPE_I_LAYER, i_layer_imp));


static void cst_abs_layer_set_root_i (CstLayer *o, CstLayerNode *root) {
  sys_return_if_fail(o != NULL);
  CstAbsLayer *self = CST_ABS_LAYER(o);
  CstAbsNode *bnode = CST_ABS_NODE(root);

  sys_bheap_push(&self->bheap, bnode);
}

static CstLayerNode* cst_abs_layer_get_root_i(CstLayer* o) {
  sys_return_val_if_fail(o != NULL, NULL);
  CstAbsLayer *self = CST_ABS_LAYER(o);

  return sys_bheap_peek(&self->bheap);
}

static SysBool abs_layer_mark_one(CstRenderNode* rnode, AbsLayerPass* ctx) {
  CstLayerNode* lnode;
  CstLayer* self;
  FrRegion* region;
  const FrRect* bound;

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

static void cst_abs_layer_check_i(CstLayer *o, CstLayout *layout) {
  CstAbsLayer* self = CST_ABS_LAYER(o);

  FrRegion *region = cst_layout_get_region(layout);
  AbsLayerPass ctx = { o, region };
}

static CstLayerNode *cst_abs_layer_new_node_i(CstLayer *layer) {
  return cst_abs_node_new_I(layer);
}

static void cst_abs_layer_append_node_i(CstLayer *o, CstLayerNode* parent, CstLayerNode* children) {
  CstAbsLayer *self = CST_ABS_LAYER(o);

  sys_bheap_push(&self->bheap, children);
}

static void cst_abs_layer_iterate_node_i(CstLayer* o,
  CstLayerNode* lnode,
  CstLayerNodeFunc func,
  SysPointer user_data) {

  CstAbsLayer *self = CST_ABS_LAYER(o);
  CstLayerNode *node = NULL;
  SysBHeapIter *iter = sys_bheap_iter_new(&self->bheap);

  while(sys_bheap_iter_next(iter, &node)) {
    if(!func(node, user_data)) {
      break;
    }
  }

  sys_bheap_iter_free(iter);
}

static void i_layer_imp(CstILayerInterface *iface) {
  iface->check = cst_abs_layer_check_i;
  iface->get_root = cst_abs_layer_get_root_i;
  iface->set_root = cst_abs_layer_set_root_i;
  iface->new_node = cst_abs_layer_new_node_i;
  iface->append_node = cst_abs_layer_append_node_i;
  iface->iterate_node = cst_abs_layer_iterate_node_i;
}

static SysDouble priority_func(SysPointer data) {
  CstAbsNode *node = data;

  return (SysDouble)(SysUInt64)(node->z_index);
}

/* object api */
CstLayer *cst_abs_layer_new(void) {
  return sys_object_new(CST_TYPE_ABS_LAYER, NULL);
}

static void cst_abs_layer_dispose(SysObject* o) {
  CstAbsLayer *self = CST_ABS_LAYER(o);

  sys_bheap_destroy(&self->bheap);

  SYS_OBJECT_CLASS(cst_abs_layer_parent_class)->dispose(o);
}

CstLayer * cst_abs_layer_new_I(void) {
  CstLayer * o = cst_abs_layer_new();
  return o;
}

static void cst_abs_layer_class_init(CstAbsLayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_abs_layer_dispose;
}

static void cst_abs_layer_init(CstAbsLayer *self) {
  sys_bheap_init(&self->bheap, priority_func, (SysDestroyFunc)_sys_object_unref);

  cst_layer_set_name(CST_LAYER(self), "AbsLayer");
}
