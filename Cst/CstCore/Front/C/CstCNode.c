#include <CstCore/Front/C/CstCNode.h>
#include <CstCore/Front/Common/CstNodeMapCore.h>
#include <CstCore/Parser/Ast.h>
#include <CstCore/Front/Common/CstComponent.h>
#include <CstCore/Front/Common/CstNode.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstLayoutContext.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
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

  FRAWatch *nwatch;
  CstNodeMap *map;

  CstNode *n = sys_object_new(type, NULL);

  n->id = sys_strdup(o->id);

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
/* sys object api */
static void cst_node_relayout_down_i(CstModule *v_module, CstNode *v_parent, CstNode *self, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

}

static CstNode* cst_node_realize_i(CstModule *v_module, CstComNode *com_node, CstNode *v_parent, CstNode *self, CstRender *v_render) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRAWatch *nwatch;
  CstRenderNode *render_node;

  render_node = cst_render_node_new_I(self);

  sys_list_foreach(self->awatches, item) {
    nwatch = item->data;

    fr_awatch_bind(nwatch, self);
  }

  if (com_node) {

    cst_node_bind(self, com_node);
  }

  return self;
}

static void cst_node_repaint_i(CstModule *v_module, CstNode *v_parent, CstNode *self, FRDraw *draw, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  // sys_assert(self->bound.x >= 0 && "node x >= 0 failed, relayout not correct ?");
  // sys_assert(self->bound.y >= 0 && "node y >= 0 failed, relayout not correct ?");
  // sys_assert(self->bound.width >= 0 && "node width >= 0 faild, relayout not correct ?");
  // sys_assert(self->bound.height >= 0 && "node height >= 0 failed, relayout not correct ?");

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
  //if (v_component) {
  //  node_set_css_props(self, v_component, (const SysChar **)v_base, v_base_len);

  //}

  // set system css
  //if (*self->name != '<') {

  //  cst_css_group_set_by_id(self->css_groups, NULL, self->name);
  //}

  if (v_props->v_absolute) {

    cst_node_set_abs_link(self, NULL);
  }

  sys_assert(self->id != NULL && "node id must be set, construct not correct ?");
}

static void cst_node_class_init(CstNodeClass *cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_node_dispose;

  cls->construct = cst_node_construct_i;
  cls->dclone = cst_node_dclone_i;
  cls->realize = cst_node_realize_i;
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
  CstCssClosure *c;

  self->id = NULL;
  self->last_child = NULL;
  self->awatches = NULL;
}

