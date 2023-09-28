#include <CstCore/Front/Common/CstNodePrivate.h>
#include <CstCore/Front/C/CstCNode.h>
#include <CstCore/Front/Common/CstNodeMapCore.h>
#include <CstCore/Parser/Ast.h>
#include <CstCore/Front/Common/CstComponent.h>
#include <CstCore/Front/Common/CstNode.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstPaintNode.h>
#include <CstCore/Driver/CstPaintContext.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstLayoutContext.h>


#define NODE_ID_FORMAT "id.%d.%d"

SYS_DEFINE_TYPE(CstNode, cst_node, SYS_TYPE_OBJECT);

CstNode* cst_node_new(void) {
  return sys_object_new(CST_TYPE_NODE, NULL);
}

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

static SYS_INLINE SysChar *node_gen_id(CstNode* node, CstModule *mod) {
  SysChar *nid = NULL;
  SysInt mid;
  SysInt ccount = 0;

  if (mod) {
    mid = cst_module_get_hashcode(mod);
    ccount = cst_module_count(mod);

  } else {
    mid = sys_str_hash((SysPointer)"croot");
  }

  nid = sys_strdup_printf(NODE_ID_FORMAT, mid, ccount);

  return nid;
}

static SysBool node_css_exists(SysPtrArray *css_list, const SysChar *node_id, CstCssGroup *g) {
  sys_return_val_if_fail(css_list == NULL, false);
  sys_return_val_if_fail(node_id == NULL, false);

  if (css_list->len == 0) { return false; }

  for(SysInt i = 0; i < (SysInt)css_list->len; i++) {
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
  if (node_css_exists(self->css_groups, self->id, g)) {
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

SysBool cst_node_set_css_r(CstNode *self, CstCssGroup *g) {
  return node_set_css_r_i(self, g);
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

CstNode* cst_node_dclone_i(CstNode *o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysType type = sys_type_from_instance(o);

  SysPtrArray *ptr;
  FRAWatch *nwatch;
  CstNodeMap *map;

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

static void cst_node_realize_r(CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *self, CstRender *v_render) {
  CstNode *nnode;

  nnode = cst_node_realize(v_module, ncomp_node, v_parent, self, v_render);
  cst_module_count_inc(v_module);

  if (nnode) {

    cst_node_append(v_parent, nnode);
  }


  if(self->children) {
    cst_node_realize_r(v_module, ncomp_node, nnode, self->children, v_render);
  }

  if(self->next) {
    cst_node_realize_r(v_module, ncomp_node, v_parent, self->next, v_render);
  }
}

void cst_node_realize_root(CstModule *v_module, CstComNode *ncomp_node, CstNode *root, CstNode *new_root, CstRender *v_render) {
  sys_return_if_fail(root != NULL);
  sys_return_if_fail(new_root != NULL);

  if (root->children) {
    cst_node_realize_r(v_module, ncomp_node, new_root, root->children, v_render);
  }
}

/* constrain */
void cst_constrain_same_width(CstNode *v_parent, CstNode *self, FRContext *cr, SysPointer data) {

  // self->bound.width = v_parent->prefer_width - self->mbp.m0 - self->mbp.m2;
}

void cst_constrain_same_height(CstNode *v_parent, CstNode *self, FRContext *cr, SysPointer data) {
  //nself->bound.height = npself->prefer_height - nself->mbp.m0 - nself->mbp.m2;
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

  if(self->id) {
    sys_free_N(self->id);
  }

  self->id = sys_strdup(id);
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

void cst_node_render_css(CstNode* self, FRContext *cr, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  if (cst_layout_state_layout(layout)) {
    // cst_node_relayout_reset(node);
  }

  cst_css_render_groups(self, self->css_groups, cr, layout);
}

void cst_node_set_abs_link(CstNode *self, FRPrioLink *nlink) {
  sys_return_if_fail(self != NULL);

  self->abs_link = nlink;
}

SysBool cst_node_is_visible(CstNode* self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_paint_node_is_visible(self->paint_node);
}

void cst_node_print_node_r(CstNode *self) {
  sys_return_if_fail(self != NULL);

  CstNode *pnode;

  if (self->parent) {
    pnode = self->parent;

    sys_debug_N("<%s,%s,%s>", cst_node_get_id(pnode), cst_node_get_name(self), cst_node_get_id(self));
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

CstNode* cst_node_deep_clone(CstNode *self) {
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

void cst_node_construct(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *self, CstNodeProps *v_props) {
  sys_return_if_fail(self != NULL);

  CstNodeClass* ncls = CST_NODE_GET_CLASS(self);

  sys_return_if_fail(ncls->construct != NULL);
  ncls->construct(v_module, v_component, v_parent, self, v_props);
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

FRPrioLink* cst_node_get_abs_node(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->abs_link;
}

SysBool cst_node_is_dirty(CstNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_layout_context_need_layout(self->layout_node) 
    || cst_paint_context_need_paint(self->paint_node);
}

static void cst_node_layout_init(CstNode *self) {
  FRSInt4 m4 = { 0 };
  
  cst_layout_node_get_mbp(self->layout_node, &m4);
  cst_layout_context_set_mbp(self->layout_ctx, &m4);
  cst_layout_context_calc_size(self->layout_ctx, self->layout_node);
}

SysBool cst_node_can_wrap(CstNode* self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_layout_context_can_wrap(self->layout_node);
}

void cst_node_relayout_v(CstModule* v_module, CstNode* v_parent, CstNode* self, FRDraw *draw, CstLayout *layout) {
}

CstNode* cst_node_realize(CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *self, CstRender *v_render) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstNodeClass* cls = CST_NODE_GET_CLASS(self);
  sys_return_val_if_fail(cls->realize != NULL, NULL);

  return cls->realize(v_module, ncomp_node, v_parent, self, v_render);
}

static void node_set_css_props(CstNode *self, CstComponent* comp, const SysChar* v_base[], SysInt v_base_len) {
  sys_return_if_fail(comp != NULL);
  sys_return_if_fail(self != NULL);

  CstCssGroup *g, *ng;
  const SysChar *pname;

  const SysChar *id = cst_node_get_id(self);
  const SysChar *comp_id = cst_component_get_id(comp);

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

  return;

fail:
  if (g != NULL) {
    cst_component_remove_css(comp, g);
  }
}

SysBool cst_node_is_abs_node(CstNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->abs_link != NULL;
}

void cst_node_add_awatch(CstNode *self, FRAWatch *awatch) {
  sys_return_if_fail(self != NULL);

  self->awatches = sys_list_prepend(self->awatches, awatch);
}

void cst_node_relayout_root(CstModule *v_module, CstNode *v_parent, CstNode *self, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstNode *v_children = cst_node_children(self);
  FRContext *cr = fr_draw_get_cr(draw);

  cst_node_render_enter(self, cr, layout);
  cst_node_relayout(v_module, v_parent, self, draw, layout);

  if(v_children) {
    cst_node_layout(v_module, self, v_children, draw, layout);
    cst_node_relayout_down(v_module, self, v_children, draw, layout);
  }

  cst_node_render_leave(self, cr, layout);
  cst_node_set_need_relayout(self, false);
}

void cst_node_paint(CstModule *v_module, CstNode *v_parent, CstNode *self, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  FRContext *cr = fr_draw_get_cr(draw);

  if (!cst_node_get_need_repaint(self)) {
    return;
  }

  cst_node_render_enter(self, cr, layout);
  cst_node_repaint(v_module, v_parent, self, draw, layout);
  cst_node_render_leave(self, cr, layout);

  cst_node_set_need_repaint(self, false);
}

static void node_repaint_node_r(CstModule *v_module, CstNode *v_parent, CstNode *self, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstNode *v_children = cst_node_children(self);
  CstNode *v_next = cst_node_next(self);

  cst_node_paint(v_module, v_parent, self, draw, layout);

  if (v_children) {
    node_repaint_node_r(v_module, self, v_children, draw, layout);
  }

  if (v_next) {
    node_repaint_node_r(v_module, v_parent, v_next, draw, layout);
  }
}

void cst_node_repaint_root(CstModule *v_module, CstNode *v_parent, CstNode *self, FRDraw *draw, CstLayout *layout) {

  node_repaint_node_r(v_module, v_parent, self, draw, layout);
}

void cst_node_render_enter(CstNode *self, FRContext *cr, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(cr != NULL);

  fr_context_save(cr);
  cst_node_render_css(self, cr, layout);
}

void cst_node_render_leave(CstNode *self, FRContext *cr, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(cr != NULL);

  fr_context_restore(cr);
}

static const SysChar* CST_NODE_PROP_NAMES[] = {
  "id","value","base","absolute", "label",
  "key_up","key_down",
};

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

void cst_node_stroke_rectangle(CstNode *self, FRDraw *draw) {
  sys_return_if_fail(self != NULL);

  FRSInt4 m4, p4;
  const FRRect* bound;

  cst_node_get_margin(self, &m4);
  cst_node_get_padding(self, &p4);

  bound = cst_node_get_bound(self);

  fr_draw_stroke_mp(draw, bound, &m4, &p4);

  return;
  sys_debug_N("repaint node: %s,%s<%d,%d,%d,%d>",
      cst_node_get_id(self),
      cst_node_get_name(self),
      bound->x,
      bound->y,
      bound->width,
      bound->height);
}

void cst_node_fill_rectangle(CstNode *self, FRDraw *draw) {
  sys_return_if_fail(self != NULL);

  const FRRect* bound = cst_node_get_bound(self);

  fr_draw_fill_rectangle(draw, bound);
}

CstLayoutNode *cst_node_get_layout_node(CstNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->layout_node;
}

/* sys object api */
static void cst_node_relayout_down_i(CstModule *v_module, CstNode *v_parent, CstNode *self, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstLayoutContext *lnode = self->layout_node;

  // cst_layout_layout_children(layout, v_parent, lnode, draw);
}

static CstNode* cst_node_realize_i(CstModule *v_module, CstComNode *com_node, CstNode *v_parent, CstNode *self, CstRender *v_render) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRAWatch *nwatch;
  CstNodePrivate* priv = self->priv;

  self->bound.x = 0;
  self->bound.y = 0;
  self->bound.width = -1;
  self->bound.height = -1;

  sys_list_foreach(self->awatches, item) {
    nwatch = item->data;

    fr_awatch_bind(nwatch, self);
  }

  if (com_node) {

    cst_node_bind(self, com_node);
  }

  self->is_abs_node = false;
  self->need_relayout = true;
  self->need_repaint = true;
  self->is_visible = true;
  self->realized = true;

  return self;
}

static void cst_node_repaint_i(CstModule *v_module, CstNode *v_parent, CstNode *self, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstNodePrivate* priv = self->priv;

  sys_assert(self->bound.x >= 0 && "node x >= 0 failed, relayout not correct ?");
  sys_assert(self->bound.y >= 0 && "node y >= 0 failed, relayout not correct ?");
  sys_assert(self->bound.width >= 0 && "node width >= 0 faild, relayout not correct ?");
  sys_assert(self->bound.height >= 0 && "node height >= 0 failed, relayout not correct ?");

  // sys_debug_N("repaint node: %s<%d,%d,%d,%d>", self->id, self->bound.x, self->bound.y, self->bound.width, self->bound.height);
}

static void cst_node_construct_i(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *self, CstNodeProps *v_props) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_module != NULL);

  sys_return_if_fail(self->name != NULL);

  if (v_props == NULL) {
    return;
  }

  SysChar **v_base = v_props->v_base;
  SysInt v_base_len = v_props->v_base_len;

  SysChar *id = self->id;

  sys_assert(id == NULL && "node build should noly once !");

  if (v_props->v_id) {
    id = sys_strdup(v_props->v_id);

  } else {
    id = node_gen_id(self, v_module);
  }

  self->id = id;
  self->awatches = v_props->v_awatches;
  self->node_maps = v_props->v_node_maps;

  // LBody has no parent component.
  if (v_component) {
    node_set_css_props(self, v_component, (const SysChar **)v_base, v_base_len);

  }

  // set system css
  if (*self->name != '<') {

    cst_css_group_set_by_id(self->css_groups, NULL, self->name);
  }

  if (v_props->v_absolute) {

    cst_node_set_abs_link(self, true);
  }

  sys_assert(self->id != NULL && "node id must be set, construct not correct ?");
}

static void cst_node_class_init(CstNodeClass *cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_node_dispose;

  cls->construct = cst_node_construct_i;
  cls->dclone = cst_node_dclone_i;
  cls->realize = cst_node_realize_i;
  cls->relayout = cst_node_relayout_i;
  cls->relayout_down = cst_node_relayout_down_i;
  cls->repaint = cst_node_repaint_i;
}

static void cst_node_dispose(SysObject* o) {
  CstNode *self = CST_NODE(o);
  sys_free_N(self->name);
  sys_free_N(self->id);

  cst_css_closure_unref(self->width_calc);
  cst_css_closure_unref(self->height_calc);

  if (self->child_width_calc) {
    cst_css_closure_unref(self->child_width_calc);
  }

  if (self->child_height_calc) {
    cst_css_closure_unref(self->child_height_calc);
  }

  sys_ptr_array_unref(self->css_groups);

  sys_list_free_full(self->awatches, (SysDestroyFunc)_sys_object_unref);
  sys_list_free_full(self->node_maps, (SysDestroyFunc)_sys_object_unref);

  SYS_OBJECT_CLASS(cst_node_parent_class)->dispose(o);
}

static void cst_node_init(CstNode *self) {
  CstNodePrivate *priv = self->priv = cst_node_get_private(self);
  CstCssClosure *c;

  self->id = NULL;
  self->last_child = NULL;
  self->css_groups = node_new_css_groups();
  self->awatches = NULL;
  self->realized = false;

  self->bound.x = -1;
  self->bound.y = -1;
  self->bound.width = -1;
  self->bound.height = -1;

  self->prefer_width = 0;
  self->prefer_height = 0;

  c = cst_css_closure_new(NULL, node_default_constraint_width, NULL);
  self->width_calc = c;

  c = cst_css_closure_new(NULL, node_default_constraint_height, NULL);
  self->height_calc = c;

  self->wrap = false;
}

