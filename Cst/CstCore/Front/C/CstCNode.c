#include <CstCore/Front/Common/CstNodePrivate.h>
#include <CstCore/Front/C/CstCNode.h>
#include <CstCore/Front/Common/CstNodeMapCore.h>
#include <CstCore/Parser/Ast.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Front/Common/CstComponent.h>
#include <CstCore/Front/Common/CstNode.h>


#define ID_FORMAT "id.%d.%d"

struct _CstNodePrivate {
  SysChar     *name;
  CstNode     *last_child;
  SysChar     *id;

  SysPtrArray *css_groups;

  /* Type: FRAWatch */
  SysList *awatches;

  /* Type: CstNodeMap */
  SysList *node_maps;

  SysList        *pass_node;
  FRPrioList     *abs_node;
  SysList        *shadow_node;

  /**
   * 0. normal.
   * 1. dirty.
   */
  SysBool  is_dirty      ;
  SysBool  is_changed    ;
  SysBool  is_visible    ;
  SysBool  realized      ;
  SysBool  breakoff      ;
  SysInt layer;
  SysInt z_index;

  FRRect bound;

  SysInt16 border[4];
  SysInt16 margin[4];
  SysInt16 padding[4];

  SysInt child_count;
  SysInt mbp0;
  SysInt mbp1;
  SysInt mbp2;
  SysInt mbp3;

  SysBool wrap;
  SysInt offset_w;
  SysInt offset_h;

  SysInt max_height;
  SysInt max_width;

  SysInt prefer_height;
  SysInt prefer_width;

  SysInt16 line_space;

  // self constraint
  CstCssClosure  *width_calc;
  CstCssClosure  *height_calc;

  // constraint for child, may be NULL.
  CstCssClosure  *child_width_calc;
  CstCssClosure  *child_height_calc;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstNode, cst_node, SYS_TYPE_OBJECT);

CstNode* cst_node_new(void) {
  return sys_object_new(CST_TYPE_NODE, NULL);
}

CstNode* cst_node_parent(CstNode *node) {
  sys_return_val_if_fail(node != NULL, NULL);

  return node->parent;
}

void cst_node_ref(CstNode *node) {
  sys_object_ref(node);
}

void cst_node_unref(CstNode *node) {
  sys_object_unref(node);
}

static void node_default_constraint_width(CstNode *v_parent, CstNode *v_node, FRContext *cr, SysPointer data) {
  sys_return_if_fail(v_node != NULL);
}

static void node_default_constraint_height(CstNode *v_parent, CstNode *v_node, FRContext *cr, SysPointer data) {
  sys_return_if_fail(v_node != NULL);
}

void cst_node_set_mbp(CstNode *node) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  priv->mbp0 = priv->margin[0] + priv->border[0] + priv->padding[0];
  priv->mbp1 = priv->margin[1] + priv->border[1] + priv->padding[1];
  priv->mbp2 = priv->margin[2] + priv->border[2] + priv->padding[2];
  priv->mbp3 = priv->margin[3] + priv->border[3] + priv->padding[3];
}

void node_layout_create(CstNodePrivate *priv) {
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

  nid = sys_strdup_printf(ID_FORMAT, mid, ccount);

  return nid;
}

static SysBool node_css_exists(CstNode *node, CstCssGroup *g) {
  CstNodePrivate *priv = node->priv;
  SysPtrArray *ptr = priv->css_groups;

  if (ptr->len == 0) {
    return false;
  }

  sys_array_foreach(CstCssGroup *, cg, ptr->pdata, ptr->len) {
    if (cg == g) {
      sys_warning_N("load duplicate css: %s, %s", priv->id, cst_css_group_get_id(g));
      return true;
    }
  }

  return false;
}

static SysBool node_set_css_r_i(CstNode *node, CstCssGroup *g) {
  const SysChar *id;

  sys_return_val_if_fail(g != NULL, false);

  id = cst_css_group_get_id(g);
  sys_return_val_if_fail(id != NULL, false);

  CstNodePrivate *priv = node->priv;
  SysPtrArray *base = cst_css_group_get_base(g);

  if (node_css_exists(node, g)) {
    return false;
  }

  sys_object_ref(g);
  sys_ptr_array_add(priv->css_groups, g);

  if (base == NULL || base->len == 0) {
    return true;
  }

  sys_array_foreach(CstCssGroup *, ng, base->pdata, base->len) {
    sys_assert(cst_css_group_get_id(ng) != NULL && "CstCssGroup id should not be null, maybe destroyed ?");

    node_set_css_r_i(node, ng);
  }

  return false;
}

SysBool cst_node_set_css_r(CstNode *node, CstCssGroup *g) {
  return node_set_css_r_i(node, g);
}

void cst_node_stoke_rectangle(CstNode *node, FRContext *cr) {
  SysInt x, y, w, h;

  CstNodePrivate *priv = node->priv;

  x = priv->bound.x + priv->margin[3];
  y = priv->bound.y + priv->margin[0];
  w = priv->bound.width + priv->padding[1] + priv->padding[3];
  h = priv->bound.height + priv->padding[0] + priv->padding[2];

  fr_context_rectangle(cr, x, y, w, h);
  fr_context_stroke(cr);
}

SysBool cst_node_can_wrap(CstNode *v_node) {
  sys_return_val_if_fail(v_node != NULL, false);

  CstNodePrivate *priv = v_node->priv;

  if (!priv->wrap) {
    return false;
  }

  if (priv->offset_w == 0) {
    return false;
  }

  return true;
}

void cst_node_relayout_h(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  SysInt mwidth, mheight;

  CstNodePrivate *priv = v_node->priv;
  CstNodePrivate *ppriv = v_parent->priv;

  mwidth = priv->bound.width + priv->mbp1 + priv->mbp3;
  mheight = priv->bound.height + priv->mbp2 + priv->mbp0;

  if (ppriv->child_width_calc) {
    ppriv->max_width = max(mwidth, ppriv->max_width);
    cst_css_closure_calc(ppriv->child_width_calc, v_parent, v_node, cr);
  }

  if (ppriv->child_height_calc) {
    ppriv->max_height = max(ppriv->max_height, mheight);
    cst_css_closure_calc(ppriv->child_height_calc, v_parent, v_node, cr);
  }

  mwidth = priv->bound.width + priv->mbp1 + priv->mbp3;
  mheight = priv->bound.height + priv->mbp2 + priv->mbp0;

  if (cst_node_can_wrap(v_parent)) {
    if (ppriv->bound.width != -1) {
      if (ppriv->offset_w + mwidth > ppriv->bound.width) {
        ppriv->offset_h += ppriv->max_height;
        ppriv->offset_w = 0;

        priv->bound.x = ppriv->bound.x + ppriv->mbp3 + ppriv->offset_w;
        priv->bound.y = ppriv->bound.y + ppriv->mbp0 + ppriv->offset_h;
      }

    } else {

      sys_warning_N("\"%s\" parent width should be set before wrap.", priv->id);
    }
  }

  ppriv->offset_w += mwidth;

  ppriv->prefer_width = max(ppriv->offset_w, ppriv->prefer_width);
  ppriv->prefer_height = ppriv->offset_h + mheight;
}

void cst_node_relayout_v(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  SysInt mwidth, mheight;

  CstNodePrivate *priv = v_node->priv;
  CstNodePrivate *ppriv = v_parent->priv;

  if (priv->bound.width == -1) {
    priv->bound.width = priv->offset_w;
  }

  if (priv->bound.height == -1) {
    priv->bound.height = priv->max_height;
  }

  mwidth = priv->bound.width + priv->mbp1 + priv->mbp3;
  mheight = priv->bound.height + priv->mbp2 + priv->mbp0;

  if (ppriv->child_width_calc) {
    ppriv->max_width = max(mwidth, ppriv->max_width);
    cst_css_closure_calc(ppriv->child_width_calc, v_parent, v_node, cr);
  }

  if (ppriv->child_height_calc) {
    ppriv->max_height = max(ppriv->max_height, mheight);
    cst_css_closure_calc(ppriv->child_height_calc, v_parent, v_node, cr);
  }

  mheight = priv->bound.height + priv->mbp2 + priv->mbp0;
  ppriv->offset_h += mheight;
}

static void cst_node_relayout_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  sys_return_if_fail(v_node != NULL);

  SysInt mwidth, mheight;
  CstNodePrivate *ppriv;
  CstNodePrivate *priv = v_node->priv;

  // priv v_parent
  if (v_parent) {
    ppriv = v_parent->priv;

    mwidth = priv->bound.width + priv->mbp1 + priv->mbp3;
    mheight = priv->bound.height + priv->mbp2 + priv->mbp0;

    ppriv->max_height = max(ppriv->max_height, mheight);
    ppriv->max_width = max(mwidth, ppriv->max_width);

    ppriv->child_count += 1;
  }
}

static void cst_node_relayout_down_i(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  SysInt mwidth, mheight;
  CstNode *nnode;

  UNUSED(mheight);
  UNUSED(mwidth);

  CstNodePrivate *priv = v_node->priv;
  CstNodePrivate *ppriv = v_parent->priv;

  SysBool need_wrap = cst_node_can_wrap(v_node->parent);

  mwidth = priv->bound.width + priv->mbp1 + priv->mbp3;
  mheight = priv->bound.height + priv->mbp2 + priv->mbp0;

  //// priv current
  if (need_wrap) {
    if (ppriv->offset_w + mwidth > ppriv->bound.width) {
      ppriv->offset_w = 0;
      ppriv->offset_h += ppriv->max_height + ppriv->line_space;

      priv->bound.x = ppriv->bound.x + ppriv->mbp3 + ppriv->offset_w;
      priv->bound.y = ppriv->bound.y + ppriv->mbp0 + ppriv->offset_h;

      ppriv->max_height = priv->bound.height;
    }

  } else {
    priv->bound.x = ppriv->bound.x + ppriv->mbp3 + ppriv->offset_w;
    priv->bound.y = ppriv->bound.y + ppriv->mbp0 + ppriv->offset_h;
  }

  nnode = v_node;
  while (nnode && nnode->children) {

    // calc child constraint
    if (ppriv->child_width_calc) {
      cst_css_closure_calc(ppriv->child_width_calc, nnode, nnode->children, cr);
    }

    if (ppriv->child_height_calc) {
      cst_css_closure_calc(ppriv->child_height_calc, nnode, nnode->children, cr);
    }

    cst_node_relayout_down(v_module, v_component, nnode, nnode->children, cr);

    nnode = nnode->next;
  }
}

CstNode *cst_node_children(CstNode *node) {
  sys_return_val_if_fail(node != NULL, NULL);

  return node->children;
}

CstNode *cst_node_next(CstNode *node) {
  sys_return_val_if_fail(node != NULL, NULL);

  return node->next;
}

static SysPtrArray *node_new_css_groups(void) {
  SysPtrArray *ptr = sys_ptr_array_new_with_free_func((SysDestroyFunc)_sys_object_unref);

  return ptr;
}

CstNode* cst_node_dclone_i(CstNode *old) {
  sys_return_val_if_fail(old != NULL, NULL);
  SysType type = sys_type_from_instance(old);

  SysPtrArray *ptr;
  FRAWatch *nwatch;
  CstNodeMap *map;
  CstNode *nnode = sys_object_new(type, NULL);
  CstNodePrivate *npriv = nnode->priv;
  CstNodePrivate *opriv = old->priv;

  npriv->id = sys_strdup(opriv->id);
  ptr = opriv->css_groups;
  for(SysUInt i = 0; i < ptr->len; i++) {
    CstCssGroup *g = cst_css_group_clone(ptr->pdata[i]);

    sys_ptr_array_add(npriv->css_groups, g);
  }

  sys_list_foreach(opriv->awatches, item) {
    nwatch = fr_awatch_clone(item->data);

    npriv->awatches = sys_list_prepend(npriv->awatches, nwatch);
  }

  sys_list_foreach(opriv->node_maps, item) {
    map = cst_node_map_clone(item->data);

    npriv->node_maps = sys_list_prepend(npriv->node_maps, map);
  }

  return nnode;
}

static void cst_node_realize_r(CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render) {
  CstNode *nnode;

  nnode = cst_node_realize(v_module, ncomp_node, v_parent, v_node, v_render);
  cst_module_count_inc(v_module);

  if (nnode) {

    cst_node_append(v_parent, nnode);
  }


  if(v_node->children) {
    cst_node_realize_r(v_module, ncomp_node, nnode, v_node->children, v_render);
  }

  if(v_node->next) {
    cst_node_realize_r(v_module, ncomp_node, v_parent, v_node->next, v_render);
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
void cst_constrain_same_width(CstNode *v_parent, CstNode *v_node, FRContext *cr, SysPointer data) {
  CstNodePrivate *npriv = v_node->priv;
  CstNodePrivate *nppriv = v_parent->priv;

  npriv->bound.width = nppriv->max_width - npriv->mbp0 - npriv->mbp2;
}

void cst_constrain_same_height(CstNode *v_parent, CstNode *v_node, FRContext *cr, SysPointer data) {
  CstNodePrivate *npriv = v_node->priv;
  CstNodePrivate *nppriv = v_parent->priv;

  npriv->bound.height = nppriv->max_height - npriv->mbp0 - npriv->mbp2;
}

/* api */
void cst_node_set_padding(CstNode *node, SysInt16 m0, SysInt16 m1, SysInt16 m2, SysInt16 m3) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  M4_SET(priv->padding, m0, m1, m2, m3);
}

void cst_node_get_padding(CstNode *node, SysInt16 *m0, SysInt16 *m1, SysInt16 *m2, SysInt16 *m3) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  M4_GET(priv->padding, m0, m1, m2, m3);
}

void cst_node_set_margin(CstNode *node, SysInt16 m0, SysInt16 m1, SysInt16 m2, SysInt16 m3) {

  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  M4_SET(priv->margin, m0, m1, m2, m3);
}

void cst_node_get_margin(CstNode *node, SysInt16 *m0, SysInt16 *m1, SysInt16 *m2, SysInt16 *m3) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  M4_GET(priv->margin, m0, m1, m2, m3);
}

void cst_node_set_child_width_closure(CstNode *node, CstCssClosure *c) {
  sys_return_if_fail(node != NULL);
  CstNodePrivate *priv = node->priv;

  if (priv->child_width_calc) {
    cst_css_closure_unref(priv->child_width_calc);
  }

  priv->child_width_calc = c;
}

void cst_node_set_child_height_closure(CstNode *node, CstCssClosure *c) {
  sys_return_if_fail(node != NULL);
  CstNodePrivate *priv = node->priv;
  
  if (priv->child_height_calc) {
    cst_css_closure_unref(priv->child_height_calc);
  }

  priv->child_height_calc = c;
}

void cst_node_set_width_closure(CstNode *node, CstCssClosure *c) {
  sys_return_if_fail(node != NULL);
  CstNodePrivate *priv = node->priv;
  
  if (priv->width_calc) {
    cst_css_closure_unref(priv->width_calc);
  }

  priv->width_calc = c;
}

void cst_node_set_height_closure(CstNode *node, CstCssClosure *c) {
  sys_return_if_fail(node != NULL);
  CstNodePrivate *priv = node->priv;
  
  if (priv->height_calc) {
    cst_css_closure_unref(priv->height_calc);
  }

  priv->height_calc = c;
}

void cst_node_set_size(CstNode *node, SysInt width, SysInt height) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  priv->bound.width = width;
  priv->bound.height = height;
}

void cst_node_get_size(CstNode *node, SysInt *width, SysInt *height) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  *width = priv->bound.width;
  *height = priv->bound.height;
}

void cst_node_get_mbp(CstNode *node, SysInt *m0, SysInt *m1, SysInt *m2, SysInt *m3) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  *m0 = priv->mbp0;
  *m1 = priv->mbp1;
  *m2 = priv->mbp2;
  *m3 = priv->mbp3;
}

void cst_node_set_prefer_size(CstNode *node, SysInt width, SysInt height) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  priv->prefer_width = width;
  priv->prefer_height = height;
}

void cst_node_get_prefer_size(CstNode *node, SysInt *width, SysInt *height) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  *width = priv->prefer_width;
  *height = priv->prefer_height;
}

SysInt cst_node_get_width(CstNode *node) {
  sys_return_val_if_fail(node != NULL, -1);
  CstNodePrivate *priv = node->priv;

  return priv->bound.width;
}

void cst_node_set_width(CstNode *node, SysInt width) {
  sys_return_if_fail(node != NULL);
  CstNodePrivate *priv = node->priv;

  priv->bound.width = width;
}

SysInt cst_node_get_height(CstNode *node) {
  sys_return_val_if_fail(node != NULL, -1);
  CstNodePrivate *priv = node->priv;

  return priv->bound.height;
}

void cst_node_set_height(CstNode *node, SysInt height) {
  sys_return_if_fail(node != NULL);
  CstNodePrivate *priv = node->priv;

  priv->bound.height = height;
}

const SysChar * cst_node_get_name(CstNode *node) {
  sys_return_val_if_fail(node != NULL, NULL);

  CstNodePrivate *priv = node->priv;

  return priv->name;
}

void cst_node_set_name(CstNode *node, const SysChar * name) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  sys_assert(priv->name == NULL);

  priv->name = sys_strdup(name);
}

const SysChar * cst_node_get_id(CstNode *node) {
  sys_return_val_if_fail(node != NULL, NULL);

  CstNodePrivate *priv = node->priv;

  return priv->id;
}

void cst_node_set_id(CstNode *node, const SysChar * id) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  if(priv->id) {
    sys_free_N(priv->id);
  }

  priv->id = sys_strdup(id);
}

SysInt cst_node_get_y(CstNode *node) {
  sys_return_val_if_fail(node != NULL, -1);

  CstNodePrivate *priv = node->priv;

  return priv->bound.y;
}

void cst_node_set_y(CstNode *node, SysInt y) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  priv->bound.y = y;
}

void cst_node_set_wrap(CstNode *node, SysBool wrap) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  priv->wrap = wrap;
}

SysInt cst_node_get_x(CstNode *node) {
  sys_return_val_if_fail(node != NULL, -1);

  CstNodePrivate *priv = node->priv;

  return priv->bound.x;
}

void cst_node_set_x(CstNode *node, SysInt x) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  priv->bound.x = x;
}

SysBool cst_node_set_css_by_id(CstNode *node, SysChar *id, CstComponent *comp) {
  sys_return_val_if_fail(id != NULL, false);
  sys_return_val_if_fail(node != NULL, false);

  CstCssGroup *g = cst_component_get_css_r(comp, id);
  if (g == NULL) {
    sys_error_N("Not found css: %s", id);
    return false;
  }

  return cst_node_set_css_r(node, g);
}

void cst_node_unlink_node_r(CstNode *node) {
  sys_return_if_fail(node != NULL);

  SysObject *o = SYS_OBJECT(node);

  if(node->children) {
    cst_node_unlink_node_r(node->children);
  }

  if(node->next) {
    cst_node_unlink_node_r(node->next);
  }

  sys_object_unref(o);
}

void cst_node_relayout_reset(CstNode *node) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  priv->bound.x = 0;
  priv->bound.y = 0;
  priv->bound.width = -1;
  priv->bound.height = -1;

  priv->child_count = 0;
  priv->mbp0 = 0;
  priv->mbp1 = 0;
  priv->mbp2 = 0;
  priv->mbp3 = 0;

  priv->wrap = false;
  priv->offset_w = 0;
  priv->offset_h = 0;

  priv->max_height = 0;
  priv->max_width = 0;

  priv->prefer_height = 0;
  priv->prefer_width = 0;

  priv->line_space = 0;

  cst_node_set_mbp(node);
}

void cst_node_render_css(CstNode* node, FRContext *cr, CST_RENDER_STATE_ENUM state) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  if (state == CST_RENDER_STATE_LAYOUT) {
    // cst_node_relayout_reset(node);
  }

  cst_css_render_groups(node, priv->css_groups, cr, state);
}

SysBool cst_node_is_changed (CstNode *node) {
  sys_return_val_if_fail(node != NULL, false);

  CstNodePrivate *priv = node->priv;

  return priv->is_changed;
}

void cst_node_set_changed(CstNode *node, SysBool changed) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  priv->is_changed = changed;
}

SysBool cst_node_get_is_dirty(CstNode *node) {
  sys_return_val_if_fail(node != NULL, false);

  CstNodePrivate *priv = node->priv;

  return priv->is_dirty;
}

void cst_node_set_is_dirty(CstNode *node, SysBool is_dirty) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  priv->is_dirty = is_dirty;
}

SysBool cst_node_layer_is(CstNode *node, SysInt state) {
  sys_return_val_if_fail(node != NULL, false);

  CstNodePrivate *priv = node->priv;

  return priv->layer == (priv->layer & state);
}

SysBool cst_node_is_visible(CstNode *node) {
  sys_return_val_if_fail(node != NULL, false);

  CstNodePrivate *priv = node->priv;

  return priv->is_visible;
}

void cst_node_set_layer(CstNode *v_node, SysInt layer) {
  sys_return_if_fail(v_node != NULL);

  CstNodePrivate* priv = v_node->priv;

  priv->layer = layer;
}

void cst_node_print_node(CstNode *node) {
  sys_return_if_fail(node != NULL);
  sys_return_if_fail(node != NULL);

  CstNode *pnode;

  if (node->parent) {
    pnode = node->parent;

    sys_debug_N("%s,%s", cst_node_get_id(pnode), cst_node_get_id(node));
  } else {

    sys_debug_N("%s", cst_node_get_id(node));
  }

  if (node->children) {
    cst_node_print_node(node->children);
  }

  if (node->next) {
    cst_node_print_node(node->next);
  }
}

CstNode* cst_node_deep_clone(CstNode *v_node) {
  sys_return_val_if_fail(v_node != NULL, NULL);

  CstNodeClass* ncls = CST_NODE_GET_CLASS(v_node);
  sys_return_val_if_fail(ncls->dclone != NULL, NULL);

  return ncls->dclone(v_node);
}

void cst_node_bind(CstNode *self, CstComNode *com_node) {
  sys_return_if_fail(self != NULL);

  CstNodeMap *map;
  CstNodePrivate *priv = self->priv;

  sys_list_foreach(priv->node_maps, list) {
    map = list->data;

    cst_node_map_bind(map, com_node, self);
  }
}

void cst_node_construct(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props) {
  sys_return_if_fail(v_node != NULL);

  CstNodeClass* ncls = CST_NODE_GET_CLASS(v_node);

  if (ncls->construct) {
    ncls->construct(v_module, v_component, v_parent, v_node, v_props);
  }
}

void cst_node_repaint(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  sys_return_if_fail(v_node != NULL);

  CstNodeClass* ncls = CST_NODE_GET_CLASS(v_node);

  if (ncls->repaint) {
    ncls->repaint(v_module, v_parent, v_node, cr);
  }
}

void cst_node_relayout(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  sys_return_if_fail(v_node != NULL);

  CstNodeClass* ncls = CST_NODE_GET_CLASS(v_node);

  if (ncls->relayout) {
    ncls->relayout(v_module, v_parent, v_node, cr);
  }
}

void cst_node_relayout_down(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  sys_return_if_fail(v_node != NULL);

  CstNodeClass* ncls = CST_NODE_GET_CLASS(v_node);

  if (ncls->relayout_down) {
    ncls->relayout_down(v_module, v_component, v_parent, v_node, cr);
  }
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

CstNode *cst_node_get_last_child(CstNode *node) {
  sys_return_val_if_fail(node != NULL, NULL);

  CstNodePrivate *priv = node->priv;

  return priv->last_child;
}

void cst_node_set_last_child(CstNode *node, CstNode *last_child) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  priv->last_child = last_child;
}

FRPrioList* cst_node_get_abs_node(CstNode *node) {
  sys_return_val_if_fail(node != NULL, NULL);

  CstNodePrivate *priv = node->priv;

  return priv->abs_node;
}

void cst_node_layout(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  sys_return_if_fail(v_node != NULL);
  sys_return_if_fail(v_parent != NULL);

  CstNodePrivate *priv = v_node->priv;
  CstNodePrivate *ppriv = v_parent->priv;

  cst_node_render_enter(v_node, cr, CST_RENDER_STATE_LAYOUT);

  if (cst_node_get_is_dirty(v_node)) {
    cst_css_closure_calc(priv->width_calc, v_parent, v_node, cr);
    cst_css_closure_calc(priv->height_calc, v_parent, v_node, cr);

    cst_node_render_css(v_node, cr, CST_RENDER_STATE_LAYOUT);
    cst_node_set_mbp(v_node);

    priv->bound.x = ppriv->bound.x + ppriv->mbp3 + ppriv->offset_w;
    priv->bound.y = ppriv->bound.y + ppriv->mbp0 + ppriv->offset_h;
  }

  if (v_node->children) {
    cst_node_layout(v_module, v_node, v_node->children, cr);
  }

  if (priv->bound.width == -1) {
    cst_node_set_width(v_node, priv->prefer_width);
  }

  if (priv->bound.height == -1) {
    cst_node_set_height(v_node, priv->prefer_height);
  }

  // priv
  cst_node_relayout(v_module, v_parent, v_node, cr);
  // sys_debug_N("%s<%d,%d,%d,%d>", priv->id, priv->bound.x, priv->bound.y, priv->bound.width, priv->bound.height);

  if (v_node->next) {
    cst_node_layout(v_module, v_parent, v_node->next, cr);
  }

  cst_node_set_is_dirty(v_node, false);
  cst_node_render_leave(v_node, cr, CST_RENDER_STATE_LAYOUT);
}

CstNode* cst_node_realize(CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render) {
  sys_return_val_if_fail(v_node != NULL, NULL);

  CstNodeClass* cls = CST_NODE_GET_CLASS(v_node);
  sys_return_val_if_fail(cls->realize != NULL, NULL);

  return cls->realize(v_module, ncomp_node, v_parent, v_node, v_render);
}

void cst_node_get_size_bp(CstNode *node, SysInt *width, SysInt *height) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate* priv = node->priv;

  *width = priv->bound.width + priv->mbp1 + priv->mbp3;
  *height = priv->bound.height + priv->mbp0 + priv->mbp2;
}

void cst_node_get_bound_bp(CstNode *node, FRRect *rect_bp) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate* priv = node->priv;

  rect_bp->x = priv->bound.x;
  rect_bp->y = priv->bound.y;
  rect_bp->width = priv->bound.width + priv->mbp1 + priv->mbp3;
  rect_bp->height = priv->bound.height + priv->mbp0 + priv->mbp2;
}

const FRRect *cst_node_get_bound(CstNode *node) {
  sys_return_val_if_fail(node != NULL, NULL);

  CstNodePrivate *priv = node->priv;

  return &priv->bound;
}

void cst_node_set_bound(CstNode *node, const FRRect *rect) {
  sys_return_if_fail(node != NULL);

  CstNodePrivate *priv = node->priv;

  priv->bound = *rect;
}

static void node_set_css_props(CstNode *node, CstComponent* comp, const SysChar* v_base[], SysInt v_base_len) {
  sys_return_if_fail(comp != NULL);
  sys_return_if_fail(node != NULL);

  CstCssGroup *g, *ng;
  const SysChar *pname;

  const SysChar *id = cst_node_get_id(node);
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
    node_set_css_r_i(node, g);
  }

  return;

fail:
  if (g != NULL) {
    cst_component_remove_css(comp, g);
  }
}

/* sys object api */
static CstNode* cst_node_realize_i(CstModule *v_module, CstComNode *com_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render) {
  sys_return_val_if_fail(v_node != NULL, NULL);

  FRAWatch *nwatch;
  CstNodePrivate* priv = v_node->priv;

  priv->bound.x = 0;
  priv->bound.y = 0;
  priv->bound.width = -1;
  priv->bound.height = -1;

  sys_list_foreach(priv->awatches, item) {
    nwatch = item->data;

    fr_awatch_bind(nwatch, v_node);
  }

  if (com_node) {

    cst_node_bind(v_node, com_node);
  }

  priv->is_changed = true;
  priv->is_dirty = true;
  priv->realized = true;

  return v_node;
}

static void cst_node_repaint_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  sys_return_if_fail(v_node != NULL);
  sys_return_if_fail(v_module != NULL);

  CstNodePrivate* priv = v_node->priv;

  sys_assert(priv->bound.x >= 0 && "node x >= 0 failed, relayout not correct ?");
  sys_assert(priv->bound.y >= 0 && "node y >= 0 failed, relayout not correct ?");
  sys_assert(priv->bound.width >= 0 && "node width >= 0 faild, relayout not correct ?");
  sys_assert(priv->bound.height >= 0 && "node height >= 0 failed, relayout not correct ?");

  sys_debug_N("repaint node: %s\t%d\t<%d,%d,%d,%d>", priv->id, sys_atomic_int_get(&(SYS_OBJECT(v_node)->ref_count)), priv->bound.x, priv->bound.y, priv->bound.width, priv->bound.height);
}

static void cst_node_construct_i(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props) {
  sys_return_if_fail(v_node != NULL);
  sys_return_if_fail(v_module != NULL);

  CstNodePrivate *priv = v_node->priv;

  sys_return_if_fail(priv->name != NULL);

  if (v_props == NULL) {
    return;
  }

  SysChar **v_base = v_props->v_base;
  SysInt v_base_len = v_props->v_base_len;

  SysChar *id = priv->id;

  sys_assert(id == NULL && "node build should noly once !");

  if (v_props->v_id) {
    id = sys_strdup(v_props->v_id);

  } else {
    id = node_gen_id(v_node, v_module);
  }

  priv->id = id;
  priv->awatches = v_props->v_awatches;
  priv->node_maps = v_props->v_node_maps;

  // LBody has no parent component.
  if (v_component) {
    node_set_css_props(v_node, v_component, (const SysChar **)v_base, v_base_len);

  }

  // set system css
  if (*priv->name != '<') {

    cst_css_group_set_by_id(priv->css_groups, NULL, priv->name);
  }

  if (v_props->v_absolute) {

    cst_node_set_layer(v_node, CST_LAYER_ABS);
  }

  sys_assert(priv->id != NULL && "node id must be set, construct not correct ?");
}

static void cst_node_class_init(CstNodeClass *ncls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(ncls);

  ocls->construct = (SysObjectFunc)cst_node_construct_i;
  ocls->dispose = cst_node_dispose;

  ncls->construct = cst_node_construct_i;
  ncls->dclone = cst_node_dclone_i;
  ncls->realize = cst_node_realize_i;
  ncls->relayout = cst_node_relayout_i;
  ncls->relayout_down = cst_node_relayout_down_i;
  ncls->repaint = cst_node_repaint_i;
}

static void cst_node_dispose(SysObject* o) {
  CstNode *node = CST_NODE(o);
  CstNodePrivate *priv = node->priv;

  sys_free_N(priv->name);
  sys_free_N(priv->id);

  cst_css_closure_unref(priv->width_calc);
  cst_css_closure_unref(priv->height_calc);

  if (priv->child_width_calc) {
    cst_css_closure_unref(priv->child_width_calc);
  }

  if (priv->child_height_calc) {
    cst_css_closure_unref(priv->child_height_calc);
  }

  sys_ptr_array_unref(priv->css_groups);
  sys_list_free_full(priv->awatches, (SysDestroyFunc)_sys_object_unref);
  sys_list_free_full(priv->node_maps, (SysDestroyFunc)_sys_object_unref);

  SYS_OBJECT_CLASS(cst_node_parent_class)->dispose(o);
}

static void cst_node_init(CstNode *self) {
  CstNodePrivate *priv = self->priv = cst_node_get_private(self);
  CstCssClosure *c;

  priv->id = NULL;
  priv->is_changed = true;
  priv->last_child = NULL;
  priv->css_groups = node_new_css_groups();
  priv->awatches = NULL;
  priv->realized = false;
  priv->is_visible = true;

  priv->bound.x = -1;
  priv->bound.y = -1;
  priv->bound.width = -1;
  priv->bound.height = -1;

  c = cst_css_closure_new(NULL, node_default_constraint_width, NULL);
  priv->width_calc = c;

  c = cst_css_closure_new(NULL, node_default_constraint_height, NULL);
  priv->height_calc = c;

  priv->wrap = false;
}

