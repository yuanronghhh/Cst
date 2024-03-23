#include <Framework/DataType/FrNode.h>

#define HNODE_TO_FR_NODE(o) SYS_HNODE_CAST_TO(o, FrNode, tree)

typedef struct _HNodePass HNodePass;

struct _HNodePass {
  FrNodeFunc func;
  SysPointer user_data;
};

SYS_DEFINE_TYPE(FrNode, fr_node, SYS_TYPE_OBJECT);


SysBool hnode_handle(SysHNode* o, SysPointer user_data) {
  HNodePass *pass = user_data;
  FrNode* self = HNODE_TO_FR_NODE(o);

  return pass->func(self, pass->user_data);
}

void fr_node_handle_bfs_r(FrNode *self, FrNodeFunc func, SysPointer user_data) {
  sys_return_if_fail(self != NULL);
  HNodePass pass = { .func = func, .user_data = user_data };

  sys_hnode_handle_bfs_r(&self->tree, (SysHNodeFunc)hnode_handle, &pass);
}

void fr_node_handle_node_ff_r(FrNode *self, FrNodeFunc func, SysPointer user_data) {
  sys_return_if_fail(self != NULL);
  HNodePass pass = { .func = func, .user_data = user_data };

  sys_hnode_handle_ff_r(&self->tree, (SysHNodeFunc)hnode_handle, &pass);
}

void fr_node_handle_ft_r(FrNode *self, FrNodeFunc func, SysPointer user_data) {
  sys_return_if_fail(self != NULL);
  HNodePass pass = { .func = func, .user_data = user_data };

  sys_hnode_handle_ft_r(&self->tree, (SysHNodeFunc)hnode_handle, &pass);
}

void fr_node_append(FrNode *parent, FrNode *node) {
  sys_return_if_fail(parent != NULL);
  sys_return_if_fail(node != NULL);

  sys_hnode_append(&parent->tree, &node->tree);
}

FrNode* fr_node_insert_after (FrNode *parent, FrNode *sibling, FrNode *node) {
  sys_return_val_if_fail (parent != NULL, node);
  sys_return_val_if_fail (node != NULL, node);

  return node;
}

void fr_node_set_last_child(FrNode *self, FrNode * last_child) {
  sys_return_if_fail(self != NULL);

  sys_hnode_set_last_child(&self->tree, &last_child->tree);
}

FrNode * fr_node_get_last_child(FrNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return HNODE_TO_FR_NODE(sys_hnode_get_last_child(&self->tree));
}

void fr_node_set_parent(FrNode *self, FrNode * parent) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(parent != NULL);

  sys_hnode_set_parent(&self->tree, &parent->tree);
}

FrNode * fr_node_get_parent(FrNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return HNODE_TO_FR_NODE(sys_hnode_parent(&self->tree));
}

FrNode* fr_node_get_children(FrNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return HNODE_TO_FR_NODE(sys_hnode_children(&self->tree));
}

FrNode* fr_node_get_next(FrNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return HNODE_TO_FR_NODE(sys_hnode_next(&self->tree));
}


FrNode* fr_node_get_prev(FrNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return HNODE_TO_FR_NODE(sys_hnode_prev(&self->tree));
}

/* object api */
static void fr_node_construct(FrNode *self) {
}

FrNode* fr_node_new(void) {
  return sys_object_new(FR_TYPE_NODE, NULL);
}

FrNode *fr_node_new_I(void) {
  FrNode *o = fr_node_new();

  fr_node_construct(o);

  return o;
}

static void fr_node_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_node_parent_class)->dispose(o);
}

static void fr_node_class_init(FrNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_node_dispose;
}

void fr_node_init(FrNode *self) {
  sys_hnode_init(&self->tree);
}
