#include "CstBoxNode.h"
#include <CstCore/Driver/CstBoxNode.h>

#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstRow.h>
#include <CstCore/Driver/CstLayerNode.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstRenderContext.h>


#define BOX_NODE_RNODE(o) (o->rnode)
#define TREE_TO_BOXNODE(tree) SYS_STRUCT_MEMBER(CstBoxNode *, tree, -(SysInt)offsetof(CstBoxNode, tree_node))
#define TREE_TO_LAYERNODE(tree) CST_LAYER_NODE(SYS_STRUCT_MEMBER_P(tree, -(SysInt)offsetof(CstBoxNode, tree_node)))

typedef struct _BoxNodePass BoxNodePass;

struct _BoxNodePass {
  CstLayerNodeFunc func;
  SysPointer user_data;
};

SYS_DEFINE_TYPE(CstBoxNode, cst_box_node, CST_TYPE_LAYER_NODE);


CstBoxNode *cst_box_node_get_last_child(CstBoxNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return TREE_TO_BOXNODE(fr_node_get_last_child(&self->tree_node));
}

void cst_box_node_set_last_child(CstBoxNode *self, CstBoxNode *last_child) {
  sys_return_if_fail(self != NULL);

  fr_node_set_last_child(&self->tree_node, &last_child->tree_node);
}

CstBoxNode* cst_box_node_insert_after(CstBoxNode *parent, CstBoxNode *sibling, CstBoxNode *box_node) {
  sys_return_val_if_fail (parent != NULL, NULL);
  sys_return_val_if_fail (box_node != NULL, NULL);

  return TREE_TO_BOXNODE(fr_node_insert_after(&parent->tree_node, &sibling->tree_node, &box_node->tree_node));
}

void cst_box_node_append(CstBoxNode *parent, CstBoxNode *box_node) {
  sys_return_if_fail(parent != NULL);
  sys_return_if_fail(box_node != NULL);

  CstBoxNode *last_child = cst_box_node_get_last_child(parent);
  cst_box_node_insert_after(parent, last_child, box_node);

  cst_box_node_set_last_child(parent, box_node);
}

void cst_box_node_layout_children(CstBoxNode *self, CstRenderContext *rctx, CstLayout *layout) {
#if 0
  CstBoxNode *cnode;
  CstLayoutNode* lnode;
  CstNode* node;
  CstRow* row;
  SysSList* rows;
  const FRRect* rbound;

  node = cst_layer_node_get_node(CST_LAYER_NODE(self));
  lnode = CST_LAYOUT_NODE(node);
  row = cst_row_new_I(lnode->bound.x, lnode->bound.y);
  rows = sys_slist_alloc();

  for(cnode = self->children; cnode; cnode = cnode->next) {
    rbound = cst_row_get_bound(row);

    if(cst_render_context_check_wrap(rctx, rbound)) {
      sys_slist_prepend(rows, row);
    }

    cst_row_add(row, lnode);
  }
#endif
}

SysBool cst_box_node_has_one_child(CstBoxNode* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->tree_node.children != NULL && self->tree_node.next == NULL;
}

void cst_box_node_relayout_node(CstBoxNode* self, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

  CstLayerNode *lnode;
  CstRenderNode *rnode;
  CstRenderContext* rctx;

  lnode = CST_LAYER_NODE(self);
  rnode = cst_layer_node_get_rnode(lnode);
  rctx = cst_render_node_get_render_ctx(rnode);

  cst_render_context_layout_box_node(rctx, self, layout);
}

void cst_box_node_relayout_root(CstBoxNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  cst_box_node_relayout_node(self, layout);
}

void cst_box_node_paint(CstBoxNode *self, CstLayout *layout) {
}

void cst_box_node_repaint_root(CstBoxNode *self, CstLayout *layout) {
}

SysBool box_node_cb(FRNode *node, SysPointer user_data) {
  BoxNodePass *pass = user_data;
  CstLayerNode *lnode = TREE_TO_LAYERNODE(node);

  return pass->func(lnode, pass->user_data);
}

void cst_box_node_bfs_handle(CstBoxNode* self, CstLayerNodeFunc func, SysPointer user_data) {
  BoxNodePass pass = { func, user_data };
  fr_node_handle_bfs_r(&self->tree_node, box_node_cb, &pass);
}

void cst_box_node_handle_r(CstBoxNode *self, CstLayerNodeFunc func, SysPointer user_data) {
  sys_return_if_fail(self != NULL);
  BoxNodePass pass = { func, user_data };

  fr_node_handle_node_ft_r(&self->tree_node, box_node_cb, &pass);
}

CstBoxNode* cst_box_node_get_parent(CstBoxNode* o) {
  sys_return_val_if_fail(o != NULL, NULL);
  CstBoxNode* self = CST_BOX_NODE(o);

  return TREE_TO_BOXNODE(self->tree_node.parent);
}

void cst_box_node_print(CstBoxNode* self, SysPointer user_data) {
  sys_return_if_fail(self != NULL);
  CstLayerNode *lnode = CST_LAYER_NODE(self);
  CstLayerNode *pnode = CST_LAYER_NODE(cst_box_node_get_parent(self));

  cst_render_node_print(
    cst_layer_node_get_rnode(lnode),
    cst_layer_node_get_rnode(pnode));
}

CstBoxNode* cst_box_node_children(CstBoxNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return TREE_TO_BOXNODE(self->tree_node.children);
}

CstBoxNode* cst_box_node_next(CstBoxNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return TREE_TO_BOXNODE(self->tree_node.next);
}

CstBoxNode* cst_box_node_parent(CstBoxNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return TREE_TO_BOXNODE(self->tree_node.parent);
}

/* object api */
CstLayerNode* cst_box_node_new(void) {
  return sys_object_new(CST_TYPE_BOX_NODE, NULL);
}

static void cst_box_node_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(cst_box_node_parent_class)->dispose(o);
}

static void cst_box_node_construct(CstLayerNode* o, CstNode *node) {

  CST_LAYER_NODE_CLASS(cst_box_node_parent_class)->construct(o, node);
}

CstLayerNode *cst_box_node_new_I(CstNode *node) {
  CstLayerNode *o = cst_box_node_new();

  cst_box_node_construct(o, node);

  return o;
}

static void cst_box_node_class_init(CstBoxNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstLayerNodeClass *ncls = CST_LAYER_NODE_CLASS(cls);

  ocls->dispose = cst_box_node_dispose;
  ncls->construct = cst_box_node_construct;
}

static void cst_box_node_init(CstBoxNode *self) {
}

