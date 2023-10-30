#include <Framework/DataType/FRNode.h>


SYS_DEFINE_TYPE(FRNode, fr_node, SYS_TYPE_OBJECT);


void fr_node_handle_bfs_r(FRNode *self, FRNodeFunc func, SysPointer user_data) {
  FRNode* node;
  FRNode* nnode;
  SysQueue* queue = sys_queue_new();

  sys_queue_push_head(queue, (SysPointer)self);

  while (!sys_queue_is_empty(queue)) {
    node = sys_queue_pop_tail(queue);

    if(!func(node, user_data)) {
      break;
    }

    for(nnode = node->children; nnode; nnode = nnode->next) {

      sys_queue_push_head(queue, nnode);
    }
  }

  sys_queue_free(queue);
}

void fr_node_handle_node_ff_r(FRNode *self, FRNodeFunc func, SysPointer user_data) {
  sys_return_if_fail(self != NULL);

  func(self, user_data);

  if (self->children) {

    fr_node_handle_node_ff_r(self->children, func, user_data);
  }

  if (self->next) {

    fr_node_handle_node_ff_r(self->next, func, user_data);
  }
}

void fr_node_handle_node_ft_r(FRNode *self, FRNodeFunc func, SysPointer user_data) {
  sys_return_if_fail(self != NULL);

  if (self->children) {

    fr_node_handle_node_ft_r(self->children, func, user_data);
  }

  if (self->next) {

    fr_node_handle_node_ft_r(self->next, func, user_data);
  }

  func(self, user_data);
}

void fr_node_append(FRNode *parent, FRNode *node) {
  sys_return_if_fail(parent != NULL);
  sys_return_if_fail(node != NULL);

  FRNode *last_child = fr_node_get_last_child(parent);
  fr_node_insert_after(parent, last_child, node);

  fr_node_set_last_child(parent, node);
}

FRNode* fr_node_insert_after (FRNode *parent, FRNode *sibling, FRNode *node) {
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

void fr_node_set_last_child(FRNode *self, FRNode * last_child) {
  sys_return_if_fail(self != NULL);

  self->last_child = last_child;
}

FRNode * fr_node_get_last_child(FRNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->last_child;
}

/* object api */
static void fr_node_construct(FRNode *self) {
}

FRNode* fr_node_new(void) {
  return sys_object_new(FR_TYPE_NODE, NULL);
}

FRNode *fr_node_new_I(void) {
  FRNode *o = fr_node_new();

  fr_node_construct(o);

  return o;
}

static void fr_node_dispose(SysObject* o) {
  FRNode *self = FR_NODE(o);

  SYS_OBJECT_CLASS(fr_node_parent_class)->dispose(o);
}

static void fr_node_class_init(FRNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_node_dispose;
}

void fr_node_init(FRNode *self) {
}
