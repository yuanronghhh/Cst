#include <CstCore/Driver/CstNode.h>

#include <CstCore/Front/Common/CstLBody.h>
#include <CstCore/Front/Common/CstLBox.h>
#include <CstCore/Front/Common/CstComNode.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/CstLayerNode.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstSurface.h>



static const SysChar* CST_NODE_PROP_NAMES[] = {
  "id","value","base","layer", "label",
  "key_up","key_down",
};


SYS_DEFINE_TYPE(CstNode, cst_node, FR_TYPE_NODE);

SysObject* cst_node_dclone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  CstNode *nself;
  CstNode *oself;
  SysObject* n;

  n = SYS_OBJECT_CLASS(cst_node_parent_class)->dclone(o);

  oself = CST_NODE(o);
  nself = CST_NODE(n);

  nself->id = sys_strdup(oself->id);
  sys_assert(nself->name != NULL);

  return n;
}

void cst_node_set_rnode_type(CstNode *self, SysType rnode_type) {
  sys_return_if_fail(self != NULL);

  self->rnode_type = rnode_type;
}

SysType cst_node_get_rnode_type(CstNode *self) {
  sys_return_val_if_fail(self != NULL, 0);

  return self->rnode_type;
}

SysBool node_unlink_one(CstNode *self, SysPointer user_data) {
  sys_return_val_if_fail(self != NULL, false);

  sys_object_unref(self);
  return true;
}

void cst_node_unlink_node_r(CstNode *self) {
  sys_return_if_fail(self != NULL);

  cst_node_handle_ft_r(self, (CstNodeFunc)node_unlink_one, NULL);
}

void cst_node_add_awatch(CstNode *self, FRAWatch* o) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(o != NULL);

  self->v_awatch_list = sys_list_prepend(self->v_awatch_list, o);
}

void cst_node_set_v_nodemap_list(CstNode *self, SysList * v_nodemap_list) {
  sys_return_if_fail(self != NULL);

  self->v_nodemap_list = v_nodemap_list;
}

void cst_node_add_nodemap(CstNode *self, CstNodeMap* o) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(o != NULL);

  self->v_nodemap_list = sys_list_prepend(self->v_nodemap_list, o);
}

SysList * cst_node_get_v_nodemap_list(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_nodemap_list;
}

void cst_node_set_v_css_list(CstNode *self, SysHArray * v_css_list) {
  sys_return_if_fail(self != NULL);

  self->v_css_list = v_css_list;
}

SysHArray * cst_node_get_v_css_list(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_css_list;
}

void cst_node_set_v_layer_idx(CstNode *self, SysInt v_layer_idx) {
  sys_return_if_fail(self != NULL);

  self->v_layer_idx = v_layer_idx;
}

SysInt cst_node_get_v_layer_idx(CstNode *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->v_layer_idx;
}

void cst_node_set_v_awatch_list(CstNode *self, SysList * v_awatch_list) {
  sys_return_if_fail(self != NULL);

  self->v_awatch_list = v_awatch_list;
}

SysList * cst_node_get_v_awatch_list(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_awatch_list;
}

void cst_node_set_v_value(CstNode *self, const SysChar * v_value) {
  sys_return_if_fail(self != NULL);

  if(self->v_value) {
    sys_clear_pointer(&self->v_value, sys_free);
  }

  self->v_value = sys_strdup(v_value);
}

SysChar * cst_node_get_v_value(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_value;
}

void cst_node_set_v_label(CstNode *self, SysChar * v_label) {
  sys_return_if_fail(self != NULL);

  if(self->v_label) {
    sys_clear_pointer(&self->v_label, sys_free);
  }

  self->v_label = sys_strdup(v_label);
}

SysChar * cst_node_get_v_label(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_label;
}

void cst_node_set_v_z_index(CstNode *self, SysInt v_z_index) {
  sys_return_if_fail(self != NULL);

  self->v_z_index = v_z_index;
}

SysInt cst_node_get_v_z_index(CstNode *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->v_z_index;
}

CstNode* cst_node_new_with_rnode_type(const SysChar* name, SysType rnode_type) {
  CstNode* self;

  self = cst_node_new();
  self->name = sys_strdup(name);
  self->rnode_type = rnode_type;
  self->v_css_list = NULL;

  return self;
}

/* api */
const SysChar * cst_node_get_name(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->name;
}

void cst_node_set_name(CstNode *self, const SysChar * name) {
  sys_return_if_fail(self != NULL);

  sys_assert(self->name == NULL);

  self->name = sys_strdup(name);
}

const SysChar * cst_node_get_id(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->id;
}

void cst_node_set_id(CstNode *self, const SysChar * id) {
  sys_return_if_fail(self != NULL);

  sys_assert(self->id == NULL);

  self->id = sys_strdup(id);
}

SysBool cst_node_print_node(CstNode* node, SysPointer user_data) {
  sys_return_val_if_fail(node != NULL, false);
  CstNode* pnode = cst_node_get_parent(node);
  
  if (pnode) {
    sys_debug_N("<%s,%s> <%s,%s>", 
      cst_node_get_name(pnode), cst_node_get_id(pnode),
      cst_node_get_name(node), cst_node_get_id(node));

  } else {
    sys_debug_N("<%s,%s>",
      cst_node_get_name(node), cst_node_get_id(node));
  }

  return true;
}

void cst_node_print_r(CstNode* node, SysPointer user_data) {
  sys_return_if_fail(node != NULL);

  cst_node_handle_bfs_r(node, (CstNodeFunc)cst_node_print_node, user_data);
}

CstRenderNode* cst_node_realize(CstNode *self, CstRenderNode *v_parent, CstComNode *com_node) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstNodeClass* ncls = CST_NODE_GET_CLASS(self);
  sys_return_val_if_fail(ncls->realize != NULL, NULL);

  return ncls->realize(self, v_parent, com_node);
}

CST_NODE_PROP_ENUM cst_node_prop_get_by_name(const SysChar * name) {
  sys_return_val_if_fail(name != NULL, -1);

  const SysChar* item = CST_NODE_PROP_NAMES[0];
  SysInt i;
  for (i = 0; i < ARRAY_SIZE(CST_NODE_PROP_NAMES); i++) {
    item = CST_NODE_PROP_NAMES[i];

    if (*name == '@') {
      return CST_NODE_PROP_ACTION;
    }

    if (sys_str_equal(name, item)) {
      return i;
    }
  }

  return -1;
}

CstRenderNode *cst_node_new_render_node(CstNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  SysType tp;
  CstRenderNode *rnode;

  if(self->rnode_type == 0) {
    sys_warning_N("%s", "node must be set render_node type before realize.");
    return NULL;
  }

  tp = self->rnode_type;
  rnode = sys_object_new(tp, NULL);

  CstRenderNodeContext param = {0};
  param.id = self->id;
  param.name = self->name;

  cst_render_node_construct(rnode, &param);

  return rnode;
}

SysBool cst_node_set_surface_and_layer(CstNode* self,
  CstLayerNode* parent,
  SysInt surf_idx,
  SysInt layer_idx) {

  CstRender* render = cst_render_get_g_render();
  CstSurface* surface = cst_render_get_surface(render, surf_idx);
  if (surface == NULL) { return false; }

  CstLayer* layer = cst_surface_get_layer_by_type(surface, layer_idx);
  if (layer == NULL) { return false; }

  return true;
}

static CstRenderNode* cst_node_realize_i(CstNode* self, CstRenderNode *v_parent, CstComNode *com_node) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(self->v_layer_idx != 0, NULL);

  SysInt layer_idx;
  CstLayer *layer;
  FRAWatch *awatch;
  CstNodeMap *nmap;
  CstRenderNode *rnode;
  CstLayerNode *lnode;
  CstLayerNode *lpnode;
  CstSurface* surface;

  layer_idx = self->v_layer_idx;
  surface = cst_render_get_default_surface();
  sys_return_val_if_fail(surface != NULL && "default surface get failed", NULL);

  rnode = cst_node_new_render_node(self);
  lpnode = v_parent ? cst_render_node_get_layer_node(v_parent) : NULL;
  layer = cst_surface_get_layer_by_type(surface, layer_idx);

  lnode = cst_layer_new_node(layer, lpnode);
  cst_render_node_set_layer_node(rnode, lnode);

  // layernode owned by layer
  sys_object_ref(lnode);

  sys_list_foreach(self->v_awatch_list, item) {
    awatch =  FR_AWATCH(item->data);
    fr_awatch_bind(awatch, rnode);
    cst_render_node_ref_awatch(rnode, awatch);
  }

  sys_list_foreach(self->v_nodemap_list, item) {
    nmap = CST_NODE_MAP(item->data);
    cst_render_node_ref_nodemap(rnode, nmap);
  }

  if (self->v_css_list != NULL && self->v_css_list->len > 0) {
    for (SysUInt i = 0; i < self->v_css_list->len; i++) {
      CstCssGroup* o = self->v_css_list->pdata[i];
      CstCssGroup *n = (CstCssGroup *)sys_object_dclone(o);

      cst_render_node_add_v_css(rnode, n);
    }
  }

  return rnode;
}

CstRenderNode* cst_node_realize_r(CstNode *self, CstRenderNode *v_parent, CstComNode *com_node) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstRenderNode *rnode;
  CstNode *co, *no;
  rnode = cst_node_realize(self, v_parent, com_node);

  co = cst_node_get_children(self);
  if (co) {

    cst_node_realize_r(co, rnode, com_node);
  }

  no = cst_node_get_next(self);
  if (no) {

    cst_node_realize_r(no, v_parent, com_node);
  }

  return rnode;
}

void cst_node_setup(void) {
}

void cst_node_teardown(void) {
}

/* sys object api */
CstNode* cst_node_new(void) {
  return sys_object_new(CST_TYPE_NODE, NULL);
}

static void cst_node_class_init(CstNodeClass *cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_node_dispose;
  ocls->dclone = cst_node_dclone_i;

  cls->realize = cst_node_realize_i;
}

static void cst_node_dispose(SysObject* o) {
  CstNode *self = CST_NODE(o);

  sys_list_free_full(self->v_awatch_list, (SysDestroyFunc)_sys_object_unref);
  self->v_awatch_list = NULL;

  sys_list_free_full(self->v_nodemap_list, (SysDestroyFunc)_sys_object_unref);
  self->v_nodemap_list = NULL;

  if (self->v_css_list) {
    sys_harray_free(self->v_css_list, true);
    self->v_css_list = NULL;
  }

  sys_clear_pointer(&self->id, sys_free);
  sys_clear_pointer(&self->name, sys_free);

  SYS_OBJECT_CLASS(cst_node_parent_class)->dispose(o);
}

static void cst_node_init(CstNode *self) {
  self->id = NULL;
  self->v_layer_idx = CST_NODE_LAYER_BOX;
}

