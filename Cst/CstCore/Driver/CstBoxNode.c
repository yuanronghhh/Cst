#include <CstCore/Driver/CstBoxNode.h>

#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstRow.h>
#include <CstCore/Driver/CstLayerNode.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstRenderContext.h>

#define BOX_NODE_TO_HNODE(o) SYS_HNODE(&o->tree_node)
#define HNODE_TO_BOX_NODE(o) SYS_HNODE_CAST_TO(o, CstBoxNode, tree_node)

typedef struct _BoxNodePass BoxNodePass;

struct _BoxNodePass {
  CstLayerNodeFunc func;
  SysPointer user_data;
};

SYS_DEFINE_TYPE(CstBoxNode, cst_box_node, CST_TYPE_LAYER_NODE);


CstBoxNode *cst_box_node_get_last_child(CstBoxNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);
  
  SysHNode *node = sys_hnode_get_last_child(BOX_NODE_TO_HNODE(self));
  
  return HNODE_TO_BOX_NODE(node);
}

void cst_box_node_set_last_child(CstBoxNode *self, CstBoxNode *last_child) {
  sys_return_if_fail(self != NULL);

  sys_hnode_set_last_child(&self->tree_node, BOX_NODE_TO_HNODE(last_child));
}

CstBoxNode* cst_box_node_insert_after(CstBoxNode *parent, CstBoxNode *sibling, CstBoxNode *box_node) {
  sys_return_val_if_fail (parent != NULL, NULL);
  sys_return_val_if_fail (box_node != NULL, NULL);

  SysHNode *node = sys_hnode_insert_after(BOX_NODE_TO_HNODE(parent), BOX_NODE_TO_HNODE(sibling), BOX_NODE_TO_HNODE(box_node));

  return HNODE_TO_BOX_NODE(node);
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

  return sys_hnode_has_one_child(BOX_NODE_TO_HNODE(self));
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

SysBool box_node_cb(SysHNode *node, SysPointer user_data) {
  BoxNodePass *pass = user_data;
  CstBoxNode *bnode = HNODE_TO_BOX_NODE(node);

  return pass->func(CST_LAYER_NODE(bnode), pass->user_data);
}

void cst_box_node_bfs_handle(CstBoxNode* self, CstLayerNodeFunc func, SysPointer user_data) {
  BoxNodePass pass = { func, user_data };
  sys_hnode_handle_bfs_r(&self->tree_node, box_node_cb, &pass);
}

void cst_box_node_handle_r(CstBoxNode *self, CstLayerNodeFunc func, SysPointer user_data) {
  sys_return_if_fail(self != NULL);
  BoxNodePass pass = { func, user_data };

  sys_hnode_handle_node_ft_r(&self->tree_node, box_node_cb, &pass);
}

CstBoxNode* cst_box_node_get_parent(CstBoxNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return HNODE_TO_BOX_NODE(sys_hnode_parent(BOX_NODE_TO_HNODE(self)));
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

  return HNODE_TO_BOX_NODE(sys_hnode_children(BOX_NODE_TO_HNODE(self)));
}

CstBoxNode* cst_box_node_next(CstBoxNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return HNODE_TO_BOX_NODE(sys_hnode_next(BOX_NODE_TO_HNODE(self)));
}

CstBoxNode* cst_box_node_parent(CstBoxNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return HNODE_TO_BOX_NODE(sys_hnode_parent(BOX_NODE_TO_HNODE(self)));
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
  sys_hnode_init(&self->tree_node);
}

