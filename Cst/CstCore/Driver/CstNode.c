#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Front/Common/CstComNode.h>
#include <CstCore/Parser/Ast.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstNodeBuilder.h>

#include <CstCore/Front/Common/CstText.h>
#include <CstCore/Front/Common/CstLBody.h>
#include <CstCore/Front/Common/CstLGrid.h>
#include <CstCore/Front/Common/CstLBox.h>
#include <CstCore/Front/Common/CstLBoxContext.h>
#include <CstCore/Front/Common/CstLGridContext.h>


static SysMutex gnode_meta_lock;
#define NODE_META_LOCK sys_mutex_lock(&gnode_meta_lock)
#define NODE_META_UNLOCK sys_mutex_unlock(&gnode_meta_lock)
SysHashTable* gnode_meta_ht = NULL;

static const SysChar* CST_NODE_PROP_NAMES[] = {
  "id","value","base","position", "label",
  "key_up","key_down",
};

SYS_DEFINE_TYPE(CstNode, cst_node, SYS_TYPE_OBJECT);


CstNode* cst_node_parent(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->parent;
}

static void node_default_constraint_width(CstNode *v_parent, CstNode *self, FRContext *cr, SysPointer data) {
  sys_return_if_fail(self != NULL);
}

static void node_default_constraint_height(CstNode *v_parent, CstNode *self, FRContext *cr, SysPointer data) {
  sys_return_if_fail(self != NULL);
}

CstNode *cst_node_children(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->children;
}

CstNode *cst_node_prev(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->prev;
}

CstNode *cst_node_next(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->next;
}

FRAWatch *cst_node_get_awatch(CstNode *self, SysType atype, const SysChar *bind_var) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRAWatch *w;
  const SysChar *wname;

  sys_list_foreach(self->awatches, item) {
    w = item->data;
    wname = fr_awatch_get_func_name(w);
    if (wname == NULL) {
      continue;
    }

    if (sys_object_is_a(w, atype) && sys_str_equal(bind_var, fr_awatch_get_func_name(w))) {
      return w;
    }
  }

  return NULL;
}

static SysPtrArray *node_new_css_groups(void) {
  SysPtrArray *ptr = sys_ptr_array_new_with_free_func((SysDestroyFunc)_sys_object_unref);

  return ptr;
}

void cst_node_render_css(CstNode *self, CstRenderNode *rnode, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(rnode != NULL);
  sys_return_if_fail(layout != NULL);

  cst_css_render_groups(self->css_groups, rnode, layout);
}

CstNode* cst_node_dclone_i(CstNode *o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysType type = sys_type_from_instance(o);

  FRAWatch *nwatch;
  CstNodeMap *map;
  SysPtrArray *ptr;
  CstNode *n = sys_object_new(type, NULL);

  n->id = sys_strdup(o->id);
  ptr = n->css_groups;

  for(SysUInt i = 0; i < ptr->len; i++) {
    CstCssGroup *g = cst_css_group_clone(ptr->pdata[i]);

    sys_ptr_array_add(n->css_groups, g);
  }

  sys_list_foreach(o->awatches, item) {
    nwatch = fr_awatch_clone(item->data);

    n->awatches = sys_list_prepend(n->awatches, nwatch);
  }

  sys_list_foreach(o->node_maps, item) {
    map = cst_node_map_clone(item->data);

    n->node_maps = sys_list_prepend(n->node_maps, map);
  }

  return n;
}

static CstRenderNode* node_realize_render_node(CstNode *self, CstRender *v_render, CstRenderNode *v_parent) {
  sys_return_val_if_fail(self != NULL, false);

  CstLayer *layer;
  CstRenderNode* n_rnode;
  CstRenderContext *rctx = self->render_ctx;

  switch (rctx->position) {
    case CST_RENDER_NODE_BOX:
      layer = cst_render_get_box_layer(v_render);
      n_rnode = cst_box_layer_realize_node(CST_BOX_LAYER(layer), CST_BOX_NODE(v_parent), self);
      break;

    case CST_RENDER_NODE_ABS:
      // TODO
      layer = cst_render_get_abs_layer(v_render);
      n_rnode = NULL;
      break;

    default:
      sys_warning_N("unknow node position: %d", rctx->position);
      n_rnode = NULL;
      break;
  }

  return n_rnode;
}

CstRenderNode* cst_node_realize_r(CstModule *v_module, CstComNode *ncomp_node, CstRenderNode *v_parent, CstNode *self, CstRender *v_render) {
  CstLayer *layer = NULL;
  CstRenderNode *rnode;

  rnode = cst_node_realize(v_module, ncomp_node, v_parent, self, v_render);
  cst_module_count_inc(v_module);

  if (rnode == NULL) {

    sys_warning_N("failed to realize node: %s,%s", cst_node_get_name(self), cst_node_get_id(self));
  }

  if(self->children) {

    cst_node_realize_r(v_module, ncomp_node, rnode, self->children, v_render);
  }

  if(self->next) {

    cst_node_realize_r(v_module, ncomp_node, v_parent, self->next, v_render);
  }

  return rnode;
}

CstRenderNode* cst_node_realize_self(CstRenderNode* v_parent, CstNode* self, CstRender *v_render) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(v_parent != NULL, NULL);

  return node_realize_render_node(self, v_render, v_parent);
}

/* css */
static SysBool node_css_exists(SysPtrArray *css_list, const SysChar *node_id, CstCssGroup *g) {
  sys_return_val_if_fail(css_list != NULL, false);
  sys_return_val_if_fail(node_id != NULL, false);

  if (css_list->len == 0) { return false; }

  for (SysInt i = 0; i < (SysInt)css_list->len; i++) {
    CstCssGroup * cg = css_list->pdata[i];

    if (cg == g) {
      sys_warning_N("load duplicate css: %s, %s", node_id, cst_css_group_get_id(g));
      return true;
    }
  }

  return false;
}

static SysBool node_set_css_r_i(CstNode *self, CstCssGroup *g) {
  const SysChar *id;
  SysPtrArray* base;

  sys_return_val_if_fail(g != NULL, false);

  id = cst_css_group_get_id(g);
  sys_return_val_if_fail(id != NULL, false);

  base = cst_css_group_get_base(g);
  if (node_css_exists(self->css_groups, cst_node_get_id(self), g)) {
    return false;
  }

  sys_object_ref(g);
  sys_ptr_array_add(self->css_groups, g);

  if (base == NULL || base->len == 0) {
    return true;
  }

  for(SysInt i = 0; i < (SysInt)base->len; i++) {
    CstCssGroup * ng = base->pdata[i];

    sys_assert(cst_css_group_get_id(ng) != NULL && "CstCssGroup id should not be null, maybe destroyed ?");

    node_set_css_r_i(self, ng);
  }

  return false;
}

void cst_node_set_css_props(CstNode *self, CstComponent* comp, const SysChar* v_base[], SysInt v_base_len) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->name != NULL);

  CstCssGroup *g, *ng;
  const SysChar *pname;
  const SysChar *comp_id;

  const SysChar *id = cst_node_get_id(self);

  if (comp) {
    comp_id = cst_component_get_id(comp);
    g = cst_component_get_css_r(comp, id);

    if (v_base_len > 0) {

      if (g == NULL) {
        g = cst_css_group_new_I(id);
        cst_component_set_css(comp, g);
      }

      for (SysInt i = 0; i < v_base_len; i++) {
        pname = v_base[i];
        if (pname == NULL) {
          goto fail;
        }

        ng = cst_component_get_css_r(comp, pname);
        if (ng && ng != g) {
          cst_css_group_set_base_r(g, ng);

        } else {
          sys_warning_N("css \"%s\" in component \"%s\" not found", pname, comp_id);
        }
      }
    }

    if (g != NULL) {

      node_set_css_r_i(self, g);
    }
  }

  // set system css
  if (*self->name != '<') {

    cst_css_group_set_by_id(self->css_groups, NULL, self->name);
  }

  return;

fail:
  if (g != NULL) {
    cst_component_remove_css(comp, g);
  }
}

void cst_node_set_position(CstNode *self, int position) {
  sys_return_if_fail(self != NULL);

  cst_render_context_set_position(self->render_ctx, position);
}

SysBool cst_node_set_css_r(CstNode *self, CstCssGroup *g) {
  return node_set_css_r_i(self, g);
}

SysBool cst_node_set_css_by_id(CstNode *self, SysChar *id, CstComponent *comp) {
  sys_return_val_if_fail(id != NULL, false);
  sys_return_val_if_fail(self != NULL, false);

  CstCssGroup *g = cst_component_get_css_r(comp, id);
  if (g == NULL) {
    sys_error_N("Not found css: %s", id);
    return false;
  }

  return cst_node_set_css_r(self, g);
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

CstNode* cst_node_dclone(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstNodeClass* ncls = CST_NODE_GET_CLASS(self);
  sys_return_val_if_fail(ncls->dclone != NULL, NULL);

  return ncls->dclone(self);
}

void cst_node_bind(CstNode *self, CstComNode *com_node) {
  sys_return_if_fail(self != NULL);

  CstNodeMap *map;
  sys_list_foreach(self->node_maps, list) {
    map = list->data;

    cst_node_map_bind(map, com_node, self);
  }
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

CstRenderNode* cst_node_realize(CstModule *v_module, CstComNode *ncomp_node, CstRenderNode *v_parent, CstNode *self, CstRender *v_render) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstNodeClass* cls = CST_NODE_GET_CLASS(self);
  sys_return_val_if_fail(cls->realize != NULL, NULL);

  return cls->realize(v_module, ncomp_node, v_parent, self, v_render);
}

void cst_node_set_awatch_list(CstNode *self, SysList *list) {
  sys_return_if_fail(self != NULL);

  self->awatches = list;
}

void cst_node_set_node_maps_list(CstNode *self, SysList *list) {
  sys_return_if_fail(self != NULL);

  self->node_maps = list;
}

void cst_node_add_awatch(CstNode *self, FRAWatch *awatch) {
  sys_return_if_fail(self != NULL);

  self->awatches = sys_list_prepend(self->awatches, awatch);
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

  return CST_NODE_PROP_LAST;
}

static void cst_node_relayout_down_i(CstModule* v_module, CstNode* v_parent, CstNode* self, FRDraw* draw, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

}

static CstRenderNode* cst_node_realize_i(CstModule* v_module, CstComNode* com_node, CstRenderNode* v_parent, CstNode* self, CstRender* v_render) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRAWatch* nwatch;

  sys_list_foreach(self->awatches, item) {
    nwatch = item->data;

    fr_awatch_bind(nwatch, self);
  }

  if (com_node) {

    cst_node_bind(self, com_node);
  }

  return node_realize_render_node(self, v_render, v_parent);
}

static void cst_node_repaint_i(CstModule* v_module, CstNode* v_parent, CstNode* self, FRDraw* draw, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

  // sys_assert(self->bound.x >= 0 && "node x >= 0 failed, relayout not correct ?");
  // sys_assert(self->bound.y >= 0 && "node y >= 0 failed, relayout not correct ?");
  // sys_assert(self->bound.width >= 0 && "node width >= 0 faild, relayout not correct ?");
  // sys_assert(self->bound.height >= 0 && "node height >= 0 failed, relayout not correct ?");

  // sys_debug_N("repaint node: %s<%d,%d,%d,%d>", self->id, self->bound.x, self->bound.y, self->bound.width, self->bound.height);
}

static void cst_node_construct_i(CstNode *self, CstNodeBuilder *builder) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->name != NULL);
  sys_return_if_fail(builder != NULL);

  SysChar* id = self->id;
  sys_assert(id == NULL && "node build should noly once !");

  cst_node_builder_build(builder, self);

  sys_assert(self->id != NULL && "node id must be set, construct not correct ?");
}

void cst_node_set_meta(const SysChar *name, SysType stype) {
  NODE_META_LOCK;
  sys_hash_table_insert(gnode_meta_ht, (SysPointer)sys_strdup(name), (SysPointer)stype);
  NODE_META_UNLOCK;
}

SysType cst_node_get_meta(const SysChar *name) {
  sys_return_val_if_fail(name != NULL, 0);

  SysType tp;

  NODE_META_LOCK;
  tp = (SysType)sys_hash_table_lookup(gnode_meta_ht, (SysPointer)name);
  NODE_META_UNLOCK;

  return tp;
}

void cst_node_setup(void) {
  sys_mutex_init(&gnode_meta_lock);
  cst_render_context_setup();

  gnode_meta_ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, sys_free, NULL);

  cst_node_set_meta("LBox",   CST_TYPE_LBOX   );
  cst_node_set_meta("LGrid",  CST_TYPE_LGRID  );
  cst_node_set_meta("LBody",  CST_TYPE_LBODY  );
  cst_node_set_meta("Text",   CST_TYPE_TEXT   );
}

void cst_node_teardown(void) {
  sys_return_if_fail(gnode_meta_ht != NULL && "cst_node_setup must be called before use node");

  sys_clear_pointer(&gnode_meta_ht, sys_hash_table_unref);
  sys_mutex_clear(&gnode_meta_lock);
  cst_render_context_teardown();
}

CstRenderContext* cst_node_new_default_context(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);
  CstNodeClass* ncls = CST_NODE_GET_CLASS(self);

  return ncls->new_default_context(self);
}

static CstRenderContext* cst_node_new_default_context_i(CstNode* node) {

  return cst_lbox_context_new_I();
}

/* sys object api */
CstNode* cst_node_new(void) {
  return sys_object_new(CST_TYPE_NODE, NULL);
}

static void cst_node_class_init(CstNodeClass *cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_node_dispose;

  cls->construct = cst_node_construct_i;
  cls->dclone = cst_node_dclone_i;
  cls->realize = cst_node_realize_i;
  cls->new_default_context = cst_node_new_default_context_i;
}

static void cst_node_dispose(SysObject* o) {
  CstNode *self = CST_NODE(o);

  sys_list_free_full(self->awatches, (SysDestroyFunc)_sys_object_unref);
  sys_list_free_full(self->node_maps, (SysDestroyFunc)_sys_object_unref);

  sys_clear_pointer(&self->name, sys_free);
  sys_clear_pointer(&self->id, sys_free);

  SYS_OBJECT_CLASS(cst_node_parent_class)->dispose(o);
}

static void cst_node_init(CstNode *self) {
  self->id = NULL;
  self->last_child = NULL;
  self->awatches = NULL;
  self->css_groups = node_new_css_groups();
}

