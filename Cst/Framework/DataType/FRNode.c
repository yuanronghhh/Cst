#include <Framework/DataType/FRNode.h>

#define HNODE_TO_FR_NODE(o) SYS_HNODE_CAST_TO(o, FRNode, tree_node)

typedef struct _HNodePass HNodePass;

struct _HNodePass {
  FRNodeFunc func;
  SysPointer user_data;
};

SYS_DEFINE_TYPE(FRNode, fr_node, SYS_TYPE_OBJECT);


SysBool hnode_handle(SysHNode* o, SysPointer user_data) {
  HNodePass *pass = user_data;
  FRNode* self = HNODE_TO_FR_NODE(o);

  return pass->func(self, pass->user_data);
}

void fr_node_handle_bfs_r(FRNode *self, FRNodeFunc func, SysPointer user_data) {
  sys_return_if_fail(self != NULL);
  HNodePass pass = { .func = func, .user_data = user_data };

  sys_hnode_handle_bfs_r(&self->tree_node, (SysHNodeFunc)hnode_handle, &pass);
}

void fr_node_handle_node_ff_r(FRNode *self, FRNodeFunc func, SysPointer user_data) {
  sys_return_if_fail(self != NULL);
  HNodePass pass = { .func = func, .user_data = user_data };

  sys_hnode_handle_node_ff_r(&self->tree_node, (SysHNodeFunc)hnode_handle, &pass);
}

void fr_node_handle_node_ft_r(FRNode *self, FRNodeFunc func, SysPointer user_data) {
  sys_return_if_fail(self != NULL);
  HNodePass pass = { .func = func, .user_data = user_data };

  sys_hnode_handle_node_ft_r(&self->tree_node, (SysHNodeFunc)hnode_handle, &pass);
}

void fr_node_append(FRNode *parent, FRNode *node) {
  sys_return_if_fail(parent != NULL);
  sys_return_if_fail(node != NULL);

  sys_hnode_append(&parent->tree_node, &node->tree_node);
}

FRNode* fr_node_insert_after (FRNode *parent, FRNode *sibling, FRNode *node) {
  sys_return_val_if_fail (parent != NULL, node);
  sys_return_val_if_fail (node != NULL, node);


  return node;
}

void fr_node_set_last_child(FRNode *self, FRNode * last_child) {
  sys_return_if_fail(self != NULL);
}

FRNode * fr_node_get_last_child(FRNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return HNODE_TO_FR_NODE(sys_hnode_get_last_child(&self->tree_node));
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

  SYS_OBJECT_CLASS(fr_node_parent_class)->dispose(o);
}

static void fr_node_class_init(FRNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_node_dispose;
}

void fr_node_init(FRNode *self) {
  sys_hnode_init(&self->tree_node);
}
