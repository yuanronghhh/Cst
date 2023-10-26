#include "CstNode.h"
#include <CstCore/Driver/CstNode.h>

#include <CstCore/Front/Common/CstComNode.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstRenderNode.h>


static const SysChar* CST_NODE_PROP_NAMES[] = {
  "id","value","base","position", "label",
  "key_up","key_down",
};

static const SysChar* CST_NODE_POSITION_NAMES[] = {
  "box", "absolute"
};

SYS_DEFINE_TYPE(CstNode, cst_node, SYS_TYPE_OBJECT);


CST_NODE_POSITION_ENUM cst_node_position_by_name(const SysChar* name) {
  return fr_get_type_by_name(CST_NODE_POSITION_NAMES, ARRAY_SIZE(CST_NODE_POSITION_NAMES), name);
}

CstNode* cst_node_parent(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->parent;
}

CstNode *cst_node_children(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->children;
}

CstNode *cst_node_prev(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->prev;
}

void cst_node_set_builder(CstNode *self, CstNodeBuilder * builder) {
  sys_return_if_fail(self != NULL);

  self->builder = builder;
}

CstNodeBuilder * cst_node_get_builder(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->builder;
}

CstNode *cst_node_next(CstNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->next;
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

CstRenderNode* cst_node_realize_r(CstNode *self, CstRenderNode *prnode, CstLayout *layout) {
  CstRenderNode *rnode;
  CstModule *v_module;
  SysInt count;

  rnode = cst_node_realize(self, prnode, layout);
  v_module = cst_layout_get_v_module(layout);

  count = cst_module_get_count(v_module);
  cst_module_set_count(v_module, ++count);

  if (rnode == NULL) {

    sys_warning_N("failed to realize node: %s,%s", cst_node_get_name(self), cst_node_get_id(self));
  }

  if(self->children) {

    cst_node_realize_r(self->children, rnode, layout);
  }

  if(self->next) {

    cst_node_realize_r(self->next, prnode, layout);
  }

  return rnode;
}

CstRenderNode* cst_node_realize_self(CstRenderNode* prnode, CstNode* self, CstLayout *layout) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(prnode != NULL, NULL);

  CstNodeBuilder *builder = self->builder;

  return cst_node_builder_build_render_node(builder, self, prnode, layout);
}

/* css */
#if 0
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
#endif

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

CstRenderNode* cst_node_realize(CstNode* self, CstRenderNode* prnode, CstLayout *layout) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstNodeClass* cls = CST_NODE_GET_CLASS(self);
  sys_return_val_if_fail(cls->realize != NULL, NULL);

  return cls->realize(self, prnode, layout);
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

static CstRenderNode* cst_node_realize_i(CstNode* self, CstRenderNode* prnode, CstLayout *layout) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstRenderNode *rnode = cst_node_builder_build_render_node(self->builder, self, prnode, layout);

  return rnode;
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

  o->builder = builder;

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

