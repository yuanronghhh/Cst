#include <CstCore/Driver/CstNode.h>

#include <CstCore/Front/Common/CstComNode.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstRenderNode.h>


static const SysChar* CST_NODE_PROP_NAMES[] = {
  "id","value","base","layer", "label",
  "key_up","key_down",
};

static const SysChar* CST_NODE_LAYER_NAMES[] = {
  "box", "absolute"
};

SYS_DEFINE_TYPE(CstNode, cst_node, SYS_TYPE_OBJECT);


CST_NODE_LAYER_ENUM cst_node_layer_by_name(const SysChar* name) {
  return fr_get_type_by_name(CST_NODE_LAYER_NAMES, ARRAY_SIZE(CST_NODE_LAYER_NAMES), name);
}

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

CstLayerNode* cst_node_realize_r(CstNode *self, CstLayerNode *parent, CstModule *v_module, CstLayout *layout) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstLayerNode *lnode;
  SysInt count;

  lnode = cst_node_realize(self, parent, v_module, layout);

  count = cst_module_get_count(v_module);
  cst_module_set_count(v_module, ++count);

  if (lnode == NULL) {

    sys_warning_N("failed to realize node: %s,%s", cst_node_get_name(self), cst_node_get_id(self));
  }

  if(self->children) {

    cst_node_realize_r(self->children, lnode, v_module, layout);
  }

  if(self->next) {

    cst_node_realize_r(self->next, parent, v_module, layout);
  }

  return lnode;
}

void cst_node_set_rnode_type(CstNode *self, SysType rnode_type) {
  sys_return_if_fail(self != NULL);

  self->rnode_type = rnode_type;
}

SysType cst_node_get_rnode_type(CstNode *self) {
  sys_return_val_if_fail(self != NULL, 0);

  return self->rnode_type;
}

/* props */

void cst_node_set_v_module(CstNode *self, CstModule * v_module) {
  sys_return_if_fail(self != NULL);

  self->v_module = v_module;
}

CstModule * cst_node_get_v_module(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_module;
}

void cst_node_set_v_component(CstNode *self, CstComponent * v_component) {
  sys_return_if_fail(self != NULL);

  self->v_component = v_component;
}

CstComponent * cst_node_get_v_component(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_component;
}

void cst_node_set_v_pnode(CstNode *self, CstNode * v_pnode) {
  sys_return_if_fail(self != NULL);

  self->v_pnode = v_pnode;
}

CstNode * cst_node_get_v_pnode(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_pnode;
}

void cst_node_set_v_awatch_list(CstNode *self, SysList * v_awatch_list) {
  sys_return_if_fail(self != NULL);

  self->v_awatch_list = v_awatch_list;
}

SysList * cst_node_get_v_awatch_list(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_awatch_list;
}

void cst_node_set_v_nodemap_list(CstNode *self, SysList * v_nodemap_list) {
  sys_return_if_fail(self != NULL);

  self->v_nodemap_list = v_nodemap_list;
}

SysList * cst_node_get_v_nodemap_list(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_nodemap_list;
}

void cst_node_set_v_css_list(CstNode *self, SysPtrArray * v_css_list) {
  sys_return_if_fail(self != NULL);

  self->v_css_list = v_css_list;
}

SysPtrArray * cst_node_get_v_css_list(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_css_list;
}

void cst_node_set_v_id(CstNode *self, const SysChar * v_id) {
  sys_return_if_fail(self != NULL);

  if(self->v_id) {
    sys_clear_pointer(&self->v_id, sys_free);
  }

  self->v_id = sys_strdup(v_id);
}

const SysChar * cst_node_get_v_id(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_id;
}

void cst_node_set_v_layer(CstNode *self, CstLayer* v_layer) {
  sys_return_if_fail(self != NULL);

  self->v_layer = v_layer;
}

CstLayer* cst_node_get_v_layer(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_layer;
}

void cst_node_set_v_value(CstNode *self, SysChar * v_value) {
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

void cst_node_unlink_node_r(CstNode *self) {
  sys_return_if_fail(self != NULL);

  SysObject *o = SYS_OBJECT(self);

  if(self->children) {
    cst_node_unlink_node_r(self->children);
  }

  if(self->next) {
    cst_node_unlink_node_r(self->next);
  }

  sys_object_unref(o);
}

void cst_node_print_node_r(CstNode *self) {
  sys_return_if_fail(self != NULL);

  CstNode *pnode;

  if (self->parent) {
    pnode = self->parent;

    sys_debug_N("<%s,%s,%s,%s>", cst_node_get_name(pnode), cst_node_get_id(pnode), cst_node_get_name(self), cst_node_get_id(self));
  } else {

    sys_debug_N("<%s,%s>", cst_node_get_name(self), cst_node_get_id(self));
  }

  if (self->children) {
    cst_node_print_node_r(self->children);
  }

  if (self->next) {
    cst_node_print_node_r(self->next);
  }
}

void cst_node_print_node(CstNode* node) {
  sys_debug_N("<%s,%s>", cst_node_get_name(node), cst_node_get_id(node));
}

void cst_node_construct(CstNode *self, CstNodeBuilder *builder) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(builder != NULL);

  CstNodeClass* ncls = CST_NODE_GET_CLASS(self);
  sys_return_if_fail(ncls->construct != NULL);

  ncls->construct(self, builder);
}

void cst_node_append(CstNode *parent, CstNode *node) {
  sys_return_if_fail(parent != NULL);
  sys_return_if_fail(node != NULL);

  CstNode *last_child = cst_node_get_last_child(parent);
  cst_node_insert_after(parent, last_child, node);

  cst_node_set_last_child(parent, node);
}

CstNode* cst_node_insert_after (CstNode *parent, CstNode *sibling, CstNode *node) {
  sys_return_val_if_fail (parent != NULL, node);
  sys_return_val_if_fail (node != NULL, node);

  if (sibling) {
    sys_return_val_if_fail (sibling->parent == parent, node);
  }

  node->parent = parent;

  if (sibling) {
    if (sibling->next) {
      sibling->next->prev = node;
    }

    node->next = sibling->next;
    node->prev = sibling;
    sibling->next = node;

  } else {
    if (parent->children) {
      node->next = parent->children;
      parent->children->prev = node;
    }

    parent->children = node;
  }

  return node;
}

CstNode *cst_node_get_last_child(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->last_child;
}

void cst_node_set_last_child(CstNode *self, CstNode *last_child) {
  sys_return_if_fail(self != NULL);

  self->last_child = last_child;
}

void cst_node_relayout_v(CstModule* v_module, CstNode* v_parent, CstNode* self, FRDraw *draw, CstLayout *layout) {
}

CstLayerNode* cst_node_realize(CstNode* self, CstLayerNode* parent, CstModule *v_module, CstLayout *layout) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstNodeClass* cls = CST_NODE_GET_CLASS(self);
  sys_return_val_if_fail(cls->realize != NULL, NULL);

  return cls->realize(self, parent, v_module, layout);
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

  tp = self->rnode_type;
  sys_assert(tp != 0 && "node should be set render node type before realize.");

  rnode = sys_object_new(tp, NULL);
  cst_render_node_construct(rnode, self);

  return rnode;
}


CstLayerNode* cst_node_new_layer_node(CstNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(self->layer != NULL, NULL);

  return cst_layer_new_node(self->layer, self);
}

static CstLayerNode* cst_node_realize_i(CstNode* self, CstLayerNode* parent, CstLayout *layout) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstRenderNode *rnode;
  CstLayerNode *lnode;
  CstLayer *layer;

  rnode = cst_node_new_render_node(node);
  lnode = cst_node_new_layer_node(self);

  sys_list_foreach(self->v_awatch_list, item) {
    FRAWatch *o =  FR_AWATCH(item->data);
    fr_awatch_bind(o, rnode);
    cst_render_node_ref_awatch(rnode, o);
  }

  sys_list_foreach(self->v_nodemap_list, item) {
    CstNodeMap *o = CST_NODE_MAP(item->data);
    cst_render_node_ref_nodemap(rnode, o);
  }

  if (self->v_css_list != NULL && self->v_css_list->len > 0) {
    for (SysUInt i = 0; i < self->v_css_list->len; i++) {
      CstCssGroup* o = self->v_css_list->pdata[i];
      CstCssGroup *n = (CstCssGroup *)sys_object_dclone(o);

      cst_render_node_add_v_css(rnode, n);
    }
  }
}

static void cst_node_construct_i(CstNode *self, CstNodeBuilder *builder) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->name != NULL);
  sys_return_if_fail(builder != NULL);

  SysChar* id = self->id;
  sys_assert(id == NULL && "node build should noly once !");

  cst_node_builder_build_node(builder, self);
}

/* sys object api */
CstNode* cst_node_new(void) {
  return sys_object_new(CST_TYPE_NODE, NULL);
}

CstNode* cst_node_new_I(CstNodeBuilder *builder) {
  CstNode* o = cst_node_new();

  cst_node_construct_i(o, builder);

  return o;
}

static void cst_node_class_init(CstNodeClass *cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_node_dispose;
  ocls->dclone = cst_node_dclone_i;

  cls->construct = cst_node_construct_i;
  cls->realize = cst_node_realize_i;
}

static void cst_node_dispose(SysObject* o) {
  CstNode *self = CST_NODE(o);

  sys_clear_pointer(&self->name, sys_free);
  sys_clear_pointer(&self->id, sys_free);
  sys_clear_pointer(&self->builder, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_node_parent_class)->dispose(o);
}

static void cst_node_init(CstNode *self) {
  self->id = NULL;
  self->last_child = NULL;
}

