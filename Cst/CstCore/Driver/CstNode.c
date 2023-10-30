#include <CstCore/Driver/CstNode.h>

#include <CstCore/Front/Common/CstComNode.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstRenderNode.h>


static const SysChar* CST_NODE_PROP_NAMES[] = {
  "id","value","base","layer", "label",
  "key_up","key_down",
};

static const SysChar* CST_NODE_LAYER_NAMES[] = {
  "box", "absolute"
};


SYS_DEFINE_TYPE(CstNode, cst_node, FR_TYPE_NODE);


CST_NODE_LAYER_ENUM cst_node_layer_by_name(const SysChar* name) {
  return fr_get_type_by_name(CST_NODE_LAYER_NAMES, ARRAY_SIZE(CST_NODE_LAYER_NAMES), name);
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

void cst_node_set_rnode_type(CstNode *self, SysType rnode_type) {
  sys_return_if_fail(self != NULL);

  self->rnode_type = rnode_type;
}

SysType cst_node_get_rnode_type(CstNode *self) {
  sys_return_val_if_fail(self != NULL, 0);

  return self->rnode_type;
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

  sys_assert(self->id == NULL);

  self->id = sys_strdup(id);
}

void cst_node_print_node(CstNode* node, SysPointer user_data) {
  sys_debug_N("<%s,%s>", cst_node_get_name(node), cst_node_get_id(node));
}

void cst_node_construct(CstNode *self, CstNodeBuilder *builder) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(builder != NULL);

  CstNodeClass* ncls = CST_NODE_GET_CLASS(self);
  sys_return_if_fail(ncls->construct != NULL);

  ncls->construct(self, builder);
}

CstLayerNode* cst_node_realize(CstNode* self, CstNodeRealizer *pass, CstLayout *layout) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstNodeClass* cls = CST_NODE_GET_CLASS(self);
  sys_return_val_if_fail(cls->realize != NULL, NULL);

  return cls->realize(self, pass, layout);
}

CstLayerNode* cst_node_new_layer_node(CstNode* self) {
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

static CstLayerNode* cst_node_realize_i(CstNode* self, CstNodeRealizer *pass, CstLayout *layout) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstLayerNode *lnode = cst_node_builder_build_render_node(self->builder, self, pass, layout);

  return lnode;
}

static void cst_node_construct_i(CstNode *self, CstNodeBuilder *builder) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->name != NULL);
  sys_return_if_fail(builder != NULL);

  SysChar* id = self->id;
  sys_assert(id == NULL && "node build should noly once !");

  cst_node_builder_build_node(builder, self);

  self->builder = builder;
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

