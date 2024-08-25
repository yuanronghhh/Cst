#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Front/CstFrontCore.h>
#include <CstCore/Driver/FlexLayout/CstFlexContext.h>
#include <CstCore/Driver/FlexLayout/CstIFlexItem.h>
#include <CstCore/Driver/CstIComNode.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstLayer.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstSurface.h>
#include <CstCore/Driver/CstLayerNode.h>
#include <CstCore/Driver/CstILayerNode.h>
#include <CstCore/Driver/CstLayerNode.h>
#include <CstCore/Driver/CstBoxNode.h>

#define NODE_META_LOCK sys_mutex_lock(&gnode_meta_lock)
#define NODE_META_UNLOCK sys_mutex_unlock(&gnode_meta_lock)

static SysMutex gnode_meta_lock;
static SysHashTable* g_node_meta_ht = NULL;

static void i_flex_item_imp(CstIFlexItemInterface* iface);

SYS_DEFINE_WITH_CODE(CstRenderNode, cst_render_node, CST_TYPE_LAYOUT_NODE,
  SYS_IMPLEMENT_INTERFACE(CST_TYPE_I_FLEX_ITEM, i_flex_item_imp));

void cst_render_node_set_render_context(CstRenderNode *self, CstRenderContext* rctx) {
  sys_return_if_fail(self != NULL);

  self->render_context = rctx;
}

CstRenderContext* cst_render_node_get_render_context(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->render_context;
}

static SysInt i_flex_item_get_width (CstIFlexItem *item) {
  CstRenderNode* self = CST_RENDER_NODE(item);

  return cst_render_node_get_width(self);
}

CstRenderNode* i_layer_node_get_next(CstRenderNode* self) {
  CstLayerNode* cnode = cst_i_layer_node_get_next(self->layer_node);

  return cst_layer_node_get_render_node(cnode);
}

CstRenderNode* i_layer_node_get_parent(CstRenderNode* self) {
  CstLayerNode* cnode = cst_i_layer_node_get_parent(self->layer_node);

  return cst_layer_node_get_render_node(cnode);
}

static SysHArray* i_flex_item_get_nodes(CstIFlexItem *item) {
  CstRenderNode *rnode;
  CstRenderNode *self;
  SysHArray *lines;
  CstLayerNode* lnode;

  self = CST_RENDER_NODE(item);
  lines = sys_harray_new_with_free_func((SysDestroyFunc)_sys_object_unref);
  lnode = cst_i_layer_node_get_children(self->layer_node);

  for (; lnode; lnode = cst_i_layer_node_get_next(lnode)) {
    rnode = cst_layer_node_get_render_node(lnode);

    sys_object_ref(rnode);
    sys_harray_add(lines, rnode);
  }

  return lines;
}

static SysInt i_flex_item_get_direction(CstIFlexItem *item) {
  CstRenderNode* rnode = CST_RENDER_NODE(item);

  return cst_render_node_get_direction(rnode);
}

static const FrBound* i_flex_item_get_bound(CstIFlexItem* item) {
  CstRenderNode* rnode = CST_RENDER_NODE(item);

  return cst_render_node_get_bound(rnode);
}

static const SysChar* i_flex_item_get_name(CstIFlexItem* item) {
  CstRenderNode* rnode = CST_RENDER_NODE(item);

  return cst_render_node_get_name(rnode);
}

static void i_flex_item_imp(CstIFlexItemInterface *iface) {
  iface->get_name = i_flex_item_get_name;
  iface->get_bound = i_flex_item_get_bound;
  iface->get_direction = i_flex_item_get_direction;
  iface->get_width = i_flex_item_get_width;
  iface->get_nodes = i_flex_item_get_nodes;
}

void cst_render_node_prepare(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  FrSInt4 m4 = { 0 };
  CstLayoutNode *lnode = CST_LAYOUT_NODE(self);

  cst_layout_node_get_mbp(lnode, &m4);
  cst_render_context_set_mbp(self->render_context, &m4);
}

void cst_render_node_render_enter(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
  FrContext* cr = cst_surface_get_cr(self->surface);

  fr_draw_save(cr);
  cst_render_node_prepare(self, layout);

  // sys_assert(self->v_css_list->len > 0 && "node must have one css node at least.");

  cst_css_group_list_render(self->v_css_list, self, layout);
  cst_render_context_calc_size(self->render_context, layout, self);
}

void cst_render_node_render_leave(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
  FrContext* cr = cst_surface_get_cr(self->surface);

  fr_draw_restore(cr);
}

void cst_render_node_set_surface(CstRenderNode *self, CstSurface * surface) {
  sys_return_if_fail(self != NULL);

  self->surface = surface;
}

CstSurface * cst_render_node_get_surface(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->surface;
}

SysObject* cst_render_node_dclone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);
  FrAWatch *nwatch;
  CstNodeMap *nodemap;

  SysObject *n = SYS_OBJECT_CLASS(cst_render_node_parent_class)->dclone(o);

  CstRenderNode *nself = CST_RENDER_NODE(n);
  CstRenderNode *oself = CST_RENDER_NODE(o);

  nself->render_context = (CstRenderContext *)sys_object_dclone(oself->render_context);
  nself->layer_node = NULL;

  for(SysUInt i = 0; i < oself->v_css_list->len; i++) {
    CstCssGroup *g = (CstCssGroup *)sys_object_dclone(oself->v_css_list->pdata[i]);

    sys_harray_add(nself->v_css_list, g);
  }

  sys_list_foreach(oself->awatch_list, item) {
    nwatch = (FrAWatch *)sys_object_dclone(item->data);

    nself->awatch_list = sys_list_prepend(nself->awatch_list, nwatch);
  }

  sys_list_foreach(oself->nodemap_list, item) {
    nodemap = (CstNodeMap *)sys_object_dclone(item->data);

    nself->nodemap_list = sys_list_prepend(nself->nodemap_list, nodemap);
  }

  return n;
}

void cst_render_node_print(CstRenderNode *self, CstRenderNode* prnode) {
  sys_return_if_fail(self != NULL);

  const FrBound* bound = cst_render_node_get_bound(self);

  if (prnode) {
    sys_info_N("<%s,%s> <%s,%s> <%d,%d,%d,%d>",
      cst_render_node_get_name(prnode),
      cst_render_node_get_id(prnode),
      cst_render_node_get_name(self),
      cst_render_node_get_id(self),
      bound->x, bound->y, bound->width, bound->height);

  } else {

    sys_info_N("<%s,%s> <%d,%d,%d,%d>",
      cst_render_node_get_name(self),
      cst_render_node_get_id(self),
      bound->x, bound->y, bound->width, bound->height);
  }
}

SysType cst_render_node_get_node_type(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, 0);

  return sys_type_from_instance(self);
}

void cst_render_node_change_to_layer(CstRenderNode *self, CstLayer *tolayer) {
}

void cst_render_node_ref_awatch(CstRenderNode *self, FrAWatch *awatch) {
  sys_return_if_fail(self != NULL);

  self->awatch_list = sys_list_prepend(self->awatch_list, awatch);
  sys_object_ref(awatch);
}

SysList * cst_render_node_get_awatch_list(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->awatch_list;
}

void cst_render_node_ref_nodemap(CstRenderNode *self, CstNodeMap * o) {
  sys_return_if_fail(self != NULL);

  self->nodemap_list = sys_list_prepend(self->nodemap_list, (SysPointer)o);
  sys_object_ref(o);
}

SysList * cst_render_node_get_nodemap_list(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->nodemap_list;
}

void cst_render_node_add_v_css(CstRenderNode *self, CstCssGroup* o) {
  sys_return_if_fail(self != NULL);

  sys_harray_add(self->v_css_list, (SysPointer)o);
}

SysHArray* cst_render_node_get_v_css_list(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_css_list;
}

void cst_render_node_set_meta(const SysChar* name, SysType stype) {
  NODE_META_LOCK;
  sys_hash_table_insert(g_node_meta_ht, (SysPointer)sys_strdup(name), (SysPointer)stype);
  NODE_META_UNLOCK;
}

SysType cst_render_node_get_meta(const SysChar* name) {
  sys_return_val_if_fail(name != NULL, 0);

  SysType tp;

  NODE_META_LOCK;
  tp = (SysType)sys_hash_table_lookup(g_node_meta_ht, (SysPointer)name);
  NODE_META_UNLOCK;

  return tp;
}

static SysBool node_unlink_one(CstLayerNode* self, SysPointer user_data) {
  sys_return_val_if_fail(self != NULL, false);
  CstRenderNode* rnode = cst_layer_node_get_render_node(CST_LAYER_NODE(self));

  sys_object_unref(rnode);
  return true;
}

void cst_render_node_unlink_node_r(CstRenderNode* self) {
  sys_return_if_fail(self != NULL);
  CstLayerNode* lnode = self->layer_node;

  cst_i_layer_node_iterate_node(lnode, (CstLayerNodeFunc)node_unlink_one, NULL);
}

void cst_render_node_setup(void) {
  sys_assert(g_node_meta_ht == NULL);

  sys_mutex_init(&gnode_meta_lock);

  g_node_meta_ht = sys_hash_table_new_full(sys_str_hash, 
    (SysEqualFunc)sys_str_equal, sys_free, NULL);

  cst_render_node_set_meta("LBox", CST_TYPE_LBOX);
  cst_render_node_set_meta("LGrid", CST_TYPE_LGRID);
  cst_render_node_set_meta("LBody", CST_TYPE_LBODY);
  cst_render_node_set_meta("Text", CST_TYPE_TEXT);
}

void cst_render_node_teardown(void) {
  sys_assert(g_node_meta_ht != NULL && "cst_render_node_setup must be called before use node");

  sys_clear_pointer(&g_node_meta_ht, sys_hash_table_unref);
  sys_mutex_clear(&gnode_meta_lock);
}

void cst_render_node_set_name(CstRenderNode *self, const SysChar* name) {
  sys_return_if_fail(self != NULL);

  self->name = sys_strdup(name);
}

const SysChar* cst_render_node_get_name(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->name;
}

void cst_render_node_set_id(CstRenderNode *self, const SysChar* id) {
  sys_return_if_fail(self != NULL);

  self->id = sys_strdup(id);
}

const SysChar* cst_render_node_get_id(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->id;
}

void cst_render_node_set_layer_node(CstRenderNode *self, CstLayerNode * layer_node) {
  sys_return_if_fail(self != NULL);

  self->layer_node = layer_node;
}

CstLayerNode * cst_render_node_get_layer_node(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->layer_node;
}

static void cst_render_node_paint_self_i(CstRenderNode *self, CstLayout *layout) {
  FrContext *cr = cst_surface_get_cr(self->surface);
  const FrBound *bound = cst_render_node_get_bound(self);

  fr_context_fill_bound(cr, bound);
}

void cst_render_node_paint_self(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstRenderNodeClass* lcls = CST_RENDER_NODE_GET_CLASS(self);
  sys_return_if_fail(lcls->paint_self != NULL);

  lcls->paint_self(self, layout);
}

void cst_render_node_set_algorithm(CstRenderNode *self, CstAlgorithm * algorithm) {
  sys_return_if_fail(self != NULL);

  self->algorithm = algorithm;
}

CstAlgorithm * cst_render_node_get_algorithm(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->algorithm;
}

/* object api */
static void cst_render_node_dispose(SysObject* o) {
  CstRenderNode* self = CST_RENDER_NODE(o);

  sys_clear_pointer(&self->algorithm, _sys_object_unref);
  sys_clear_pointer(&self->render_context, _sys_object_unref);
  sys_harray_free(self->v_css_list, true);
  self->v_css_list = NULL;

  if (self->awatch_list) {

    sys_list_free_full(self->awatch_list, (SysDestroyFunc)_sys_object_unref);
    self->awatch_list = NULL;
  }

  sys_clear_pointer(&self->layer_node, _sys_object_unref);
  if (self->id) {

    sys_clear_pointer(&self->id, sys_free);
  }
  sys_clear_pointer(&self->name, sys_free);


}

void cst_render_node_construct(CstRenderNode* self, CstRenderNodeContext *param) {
  self->id = sys_strdup(param->id);
  self->name = sys_strdup(param->name);
  self->algorithm = sys_object_ref(param->algorithm);
  self->render_context = param->render_context;
}

CstRenderNode *cst_render_node_new(void) {
  return sys_object_new(CST_TYPE_RENDER_NODE, NULL);
}

CstRenderNode *cst_render_node_new_I(CstNode *node, CstRenderNodeContext *param) {
  CstRenderNode *o = cst_render_node_new();

  cst_render_node_construct(o, param);

  return o;
}

static void cst_render_node_class_init(CstRenderNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_render_node_dispose;
  cls->construct = cst_render_node_construct;
  cls->paint_self = cst_render_node_paint_self_i;
}

static void cst_render_node_init(CstRenderNode *self) {
  self->v_css_list = cst_css_group_list_new();
}
