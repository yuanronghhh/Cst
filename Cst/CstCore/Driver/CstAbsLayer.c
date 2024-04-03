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

  sys_pqueue_push_tail_link(&self->pqueue, &bnode->pnode);
}

static CstLayerNode* cst_abs_layer_get_root_i(CstLayer* o) {
  sys_return_val_if_fail(o != NULL, NULL);
  CstAbsLayer *self = CST_ABS_LAYER(o);

  SysPointer node = sys_pqueue_peek_head(&self->pqueue);

  return CST_LAYER_NODE(node);
}

static SysBool abs_layer_mark_one(CstRenderNode* rnode, AbsLayerPass* ctx) {
  CstLayerNode* lnode;
  CstLayer* self;
  FrRegion* region;
  const FrRect* bound;

  self = ctx->v_layer;
  region = ctx->v_region;
  lnode = cst_render_node_get_layer_node(rnode);
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
}

static CstLayerNode *cst_abs_layer_new_node_i(CstLayer *layer) {
  return cst_abs_node_new_I(layer);
}

static void cst_abs_layer_append_node_i(CstLayer *o,
    CstLayerNode* parent, 
    CstLayerNode* children) {

  CstAbsLayer *self = CST_ABS_LAYER(o);
  CstAbsNode* abs_node = CST_ABS_NODE(children);

  sys_pqueue_push_tail_link(&self->pqueue, &abs_node->pnode);
}

static void i_layer_imp(CstILayerInterface *iface) {
  iface->check = cst_abs_layer_check_i;
  iface->get_root = cst_abs_layer_get_root_i;
  iface->set_root = cst_abs_layer_set_root_i;
  iface->new_node = cst_abs_layer_new_node_i;
  iface->append_node = cst_abs_layer_append_node_i;
}

/* object api */
CstLayer *cst_abs_layer_new(void) {
  return sys_object_new(CST_TYPE_ABS_LAYER, NULL);
}

static void cst_abs_layer_dispose(SysObject* o) {
  CstAbsLayer *self = CST_ABS_LAYER(o);

  sys_pqueue_destroy(&self->pqueue, (SysDestroyFunc)_sys_object_unref);

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
  sys_pqueue_init(&self->pqueue);

  cst_layer_set_name(CST_LAYER(self), "AbsLayer");
}
