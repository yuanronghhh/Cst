#include <CstCore/Driver/CstRenderNode.h>

#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/Flex/CstFlexItem.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstLayer.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstSurface.h>
#include <CstCore/Driver/CstILayerNode.h>
#include <CstCore/Driver/CstILayer.h>
#include <CstCore/Driver/CstLayerNode.h>
#include <CstCore/Front/Common/CstText.h>
#include <CstCore/Front/Common/CstLBody.h>
#include <CstCore/Front/Common/CstLGrid.h>
#include <CstCore/Front/Common/CstLBox.h>
#include <CstCore/Front/Common/CstLBoxContext.h>
#include <CstCore/Front/Common/CstLGridContext.h>
#include <CstCore/Front/Common/CstIComNode.h>


#define NODE_META_LOCK sys_mutex_lock(&gnode_meta_lock)
#define NODE_META_UNLOCK sys_mutex_unlock(&gnode_meta_lock)

static SysMutex gnode_meta_lock;
static SysHashTable* g_node_meta_ht = NULL;

static void render_node_flex_item_imp(CstFlexItemInterface* item);
static void render_node_i_com_node_imp(CstIComNodeInterface* item);


SYS_DEFINE_WITH_CODE(CstRenderNode, cst_render_node, CST_TYPE_LAYOUT_NODE,
  SYS_IMPLEMENT_INTERFACE(CST_TYPE_FLEX_ITEM, render_node_flex_item_imp)
  SYS_IMPLEMENT_INTERFACE(CST_TYPE_I_COM_NODE, render_node_i_com_node_imp)
);

void cst_render_node_set_rctx(CstRenderNode *self, CstRenderContext* rctx) {
  sys_return_if_fail(self != NULL);

  self->rctx = rctx;
}

CstRenderContext* cst_render_node_get_rctx(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->rctx;
}

SysInt render_node_get_width (CstFlexItem *item) {
  CstRenderNode* self = CST_RENDER_NODE(item);

  return cst_render_node_get_width(self);
}

static void render_node_i_com_node_imp(CstIComNodeInterface* iface) {
}

static void render_node_flex_item_imp(CstFlexItemInterface *iface) {

  iface->get_width = render_node_get_width;
}

void cst_render_node_prepare(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  FRSInt4 m4 = { 0 };
  CstLayoutNode *lnode = CST_LAYOUT_NODE(self);

  cst_layout_node_get_mbp(lnode, &m4);
  cst_render_context_set_mbp(self->rctx, &m4);
}

void cst_render_node_render_enter(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
  FRDraw* draw = cst_layout_get_draw(layout);

  fr_draw_save(draw);
  cst_render_node_prepare(self, layout);

  // sys_assert(self->v_css_list->len > 0 && "node must have one css node at least.");

  cst_css_group_list_render(self->v_css_list, self, layout);
  cst_render_context_calc_size(self->rctx, layout, self);
}

void cst_render_node_render_leave(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
  FRDraw* draw = cst_layout_get_draw(layout);

  fr_draw_restore(draw);
}

SysObject* cst_render_node_dclone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);
  FRAWatch *nwatch;
  CstNodeMap *nodemap;

  SysObject *n = SYS_OBJECT_CLASS(cst_render_node_parent_class)->dclone(o);

  CstRenderNode *nself = CST_RENDER_NODE(n);
  CstRenderNode *oself = CST_RENDER_NODE(o);

  nself->rctx = (CstRenderContext *)sys_object_dclone(oself->rctx);
  nself->layer_node = NULL;

  for(SysUInt i = 0; i < oself->v_css_list->len; i++) {
    CstCssGroup *g = (CstCssGroup *)sys_object_dclone(oself->v_css_list->pdata[i]);

    sys_harray_add(nself->v_css_list, g);
  }

  sys_list_foreach(oself->awatch_list, item) {
    nwatch = (FRAWatch *)sys_object_dclone(item->data);

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

  const FRRect* bound = cst_render_node_get_bound(self);

  if (prnode) {
    sys_debug_N("<%s,%s> <%s,%s> <%d,%d,%d,%d>",
      cst_render_node_get_name(prnode),
      cst_render_node_get_id(prnode),
      cst_render_node_get_name(self),
      cst_render_node_get_id(self),
      bound->x, bound->y, bound->width, bound->height);

  } else {

    sys_debug_N("<%s,%s> <%d,%d,%d,%d>",
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

void cst_render_node_ref_awatch(CstRenderNode *self, FRAWatch *awatch) {
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
  CstRenderNode* rnode = cst_layer_node_get_render_node(self);

  sys_object_unref(rnode);
  return true;
}

void cst_render_node_unlink_node_r(CstRenderNode* self) {
  sys_return_if_fail(self != NULL);
  CstLayerNode* lnode = self->layer_node;
  CstLayer* layer = cst_layer_node_get_layer(lnode);

  sys_assert(CST_LAYER(layer) != NULL);

  cst_i_layer_iterate_node(layer, lnode, (CstLayerNodeFunc)node_unlink_one, NULL);
}

void cst_render_node_setup(void) {
  sys_assert(g_node_meta_ht == NULL);

  sys_mutex_init(&gnode_meta_lock);
  cst_render_context_setup();

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
  cst_render_context_teardown();
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

static void cst_render_node_paint_self_i(CstRenderNode *rnode, CstLayout *layout) {
  FRDraw *draw = cst_layout_get_draw(layout);
  const FRRect *bound = cst_render_node_get_bound(rnode);

  fr_draw_fill_bound(draw, bound);
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

  sys_clear_pointer(&self->rctx, _sys_object_unref);
  sys_clear_pointer(&self->v_css_list, sys_harray_free);

  if (self->awatch_list) {

    sys_list_free_full(self->awatch_list, (SysDestroyFunc)_sys_object_unref);
    self->awatch_list = NULL;
  }

  sys_clear_pointer(&self->layer_node, _sys_object_unref);
  if (self->id) {

    sys_clear_pointer(&self->id, sys_free);
  }
  sys_clear_pointer(&self->name, sys_free);

  SYS_OBJECT_CLASS(cst_render_node_parent_class)->dispose(o);
}

void cst_render_node_construct(CstRenderNode* self, CstRenderNodeContext *param) {
  self->id = sys_strdup(param->id);
  self->name = sys_strdup(param->name);
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
