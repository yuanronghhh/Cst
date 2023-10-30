#include <CstCore/Driver/CstBoxNode.h>

#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstRow.h>
#include <CstCore/Driver/CstLayerNode.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstRenderContext.h>


#define BOX_NODE_RNODE(o) (o->rnode)


typedef struct _BoxNodePass BoxNodePass;

struct _BoxNodePass {
  SysQueue* queue;
  CstLayerNodeFunc func;
  SysPointer user_data;
};


SYS_DEFINE_TYPE(CstBoxNode, cst_box_node, CST_TYPE_LAYER_NODE);


CstBoxNode *cst_box_node_get_last_child(CstBoxNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->last_child;
}

void cst_box_node_set_last_child(CstBoxNode *self, CstBoxNode *last_child) {
  sys_return_if_fail(self != NULL);

  self->last_child = last_child;
}

CstBoxNode* cst_box_node_insert_after (CstBoxNode *parent, CstBoxNode *sibling, CstBoxNode *box_node) {
  sys_return_val_if_fail (parent != NULL, NULL);
  sys_return_val_if_fail (box_node != NULL, NULL);

  if (sibling) {
    sys_return_val_if_fail (sibling->parent == parent, box_node);
  }

  box_node->parent = parent;

  if (sibling) {
    if (sibling->next) {
      sibling->next->prev = box_node;
    }

    box_node->next = sibling->next;
    box_node->prev = sibling;
    sibling->next = box_node;

  } else {
    if (parent->children) {
      box_node->next = parent->children;
      parent->children->prev = box_node;
    }

    parent->children = box_node;
  }

  return box_node;
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

  return self->children != NULL && self->next == NULL;
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

static void box_node_repaint_box_node_r(CstBoxNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  cst_box_node_paint(self, layout);

  if (self->children) {
    box_node_repaint_box_node_r(self->children, layout);
  }

  if (self->next) {
    box_node_repaint_box_node_r(self->next, layout);
  }
}

void cst_box_node_repaint_root(CstBoxNode *self, CstLayout *layout) {

  box_node_repaint_box_node_r(self, layout);
}

void cst_box_node_handle_r(CstBoxNode *self, BoxNodePass* ctx) {
  sys_return_if_fail(self != NULL);

  ctx->func(CST_LAYER_NODE(self), ctx->user_data);

  if (self->children) {

    cst_box_node_handle_r(self->children, ctx);
  }

  if (self->next) {

    cst_box_node_handle_r(self->next, ctx);
  }
}

void cst_box_node_bfs_handle_r(CstBoxNode *self, BoxNodePass *ctx) {
  CstBoxNode* node;
  CstBoxNode* nnode;
  SysQueue* queue = ctx->queue;

  sys_queue_push_head(queue, (SysPointer)self);

  while (!sys_queue_is_empty(queue)) {
    node = sys_queue_pop_tail(queue);

    ctx->func(CST_LAYER_NODE(node), ctx->user_data);

    for(nnode = node->children; nnode; nnode = nnode->next) {

      sys_queue_push_head(queue, nnode);
    }
  }
}

void cst_box_node_bfs_handle(CstBoxNode* self, CstLayerNodeFunc func, SysPointer user_data) {
  SysQueue *queue = sys_queue_new();

  BoxNodePass ctx = { queue, func, user_data };

  cst_box_node_bfs_handle_r(self, &ctx);

  sys_queue_free(queue);
}

static CstLayerNode* cst_box_node_get_parent_i(CstLayerNode* o) {
  sys_return_val_if_fail(o != NULL, NULL);
  CstBoxNode* self = CST_BOX_NODE(o);

  return CST_LAYER_NODE(self->parent);
}

void cst_box_node_print(CstBoxNode* self, SysPointer user_data) {
  sys_return_if_fail(self != NULL);
  CstLayerNode *lnode = CST_LAYER_NODE(self);
  CstLayerNode *pnode = CST_LAYER_NODE(self->parent);

  cst_render_node_print(
    cst_layer_node_get_rnode(lnode),
    cst_layer_node_get_rnode(pnode));
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

