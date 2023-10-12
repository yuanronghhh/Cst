#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Front/Common/CstLBoxContext.h>


typedef struct _BoxNodeContext BoxNodeContext;

struct _BoxNodeContext {
  SysQueue* queue;
  CstRenderNodeFunc func;
  SysPointer user_data;
};


SYS_DEFINE_TYPE(CstBoxNode, cst_box_node, CST_TYPE_RENDER_NODE);


CstBoxNode *cst_box_node_children(CstBoxNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->children;
}

CstBoxNode *cst_box_node_next(CstBoxNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->next;
}

CstBoxNode *cst_box_node_get_last_child(CstBoxNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->last_child;
}

void cst_box_node_set_last_child(CstBoxNode *self, CstBoxNode *last_child) {
  sys_return_if_fail(self != NULL);

  self->last_child = last_child;
}

CstBoxNode* cst_box_node_insert_after (CstBoxNode *parent, CstBoxNode *sibling, CstBoxNode *box_node) {
  sys_return_val_if_fail (parent != NULL, box_node);
  sys_return_val_if_fail (box_node != NULL, box_node);

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

void cst_box_node_relayout_children(CstBoxNode *self, CstLayout *layout) {
}

void cst_box_node_relayout_root(CstBoxNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstRenderNode *render_node = CST_RENDER_NODE(self);

  cst_render_node_render_enter(render_node, layout);
  cst_render_node_relayout_self(render_node, layout);

  if(self->children) {

    cst_box_node_relayout_children(self->children, layout);
  }

  cst_render_node_render_leave(render_node, layout);
  cst_render_node_set_layout(render_node, false);
}

void cst_box_node_paint(CstBoxNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstRenderNode *render_node = CST_RENDER_NODE(self);

  if (!cst_render_node_need_paint(render_node)) {
    return;
  }

  cst_render_node_render_enter(render_node, layout);
  cst_render_node_paint_self(render_node, layout);
  cst_render_node_render_leave(render_node, layout);

  cst_render_node_set_paint(render_node, false);
}

static void box_node_repaint_box_node_r(CstBoxNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  FRDraw *draw = cst_layout_get_draw(layout);

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

void cst_box_node_handle_r(CstBoxNode *self, BoxNodeContext* ctx) {
  sys_return_if_fail(self != NULL);

  ctx->func(CST_RENDER_NODE(self), ctx->user_data);

  if (self->children) {

    cst_box_node_handle_r(self->children, ctx);
  }

  if (self->next) {

    cst_box_node_handle_r(self->next, ctx);
  }
}

void cst_box_node_bfs_handle_r(CstBoxNode *self, BoxNodeContext *ctx) {
  CstBoxNode* node;
  CstBoxNode* nnode;
  SysQueue* queue = ctx->queue;

  sys_queue_push_head(queue, (SysPointer)self);

  while (!sys_queue_is_empty(queue)) {
    node = sys_queue_pop_tail(queue);

    ctx->func(CST_RENDER_NODE(node), ctx->user_data);

    for(nnode = node->children; nnode; nnode = nnode->next) {

      sys_queue_push_head(queue, nnode);
    }
  }
}

void cst_box_node_bfs_handle(CstBoxNode* self, CstRenderNodeFunc func, SysPointer user_data) {
  SysQueue *queue = sys_queue_new();

  BoxNodeContext ctx = { queue, func, user_data };

  cst_box_node_bfs_handle_r(self, &ctx);

  sys_queue_free(queue);
}

void cst_box_node_layout_children(CstBoxNode* self, CstLayout* layout) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(layout != NULL);

  CstBoxNode* node;
  for (node = self->children; node; node = node->next) {

    cst_render_node_layout(CST_RENDER_NODE(node), layout);
  }
}

/* object api */
CstRenderNode* cst_box_node_new(void) {
  return sys_object_new(CST_TYPE_BOX_NODE, NULL);
}

static void cst_box_node_dispose(SysObject* o) {
  CstBoxNode *self = CST_BOX_NODE(o);

  SYS_OBJECT_CLASS(cst_box_node_parent_class)->dispose(o);
}

static void cst_box_node_construct(CstRenderNode* o, CstNode *node) {

  CST_RENDER_NODE_CLASS(cst_box_node_parent_class)->construct(o, node);
}

CstRenderNode *cst_box_node_new_I(CstNode *node) {
  CstRenderNode *o = cst_box_node_new();

  cst_box_node_construct(o, node);

  return o;
}

static void cst_box_node_class_init(CstBoxNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstRenderNodeClass* rcls = CST_RENDER_NODE_CLASS(cls);

  ocls->dispose = cst_box_node_dispose;
}

static void cst_box_node_init(CstBoxNode *self) {
}

