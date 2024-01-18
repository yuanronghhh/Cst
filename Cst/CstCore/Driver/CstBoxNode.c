#include <CstCore/Driver/CstBoxNode.h>

#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstRow.h>
#include <CstCore/Driver/CstLayerNode.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstRender.h>


#define BOX_NODE_TO_HNODE(o) _box_node_to_hnode(o)
#define HNODE_TO_BOX_NODE(o) SYS_HNODE_CAST_TO(o, CstBoxNode, tree_node)

typedef struct _BoxNodePass BoxNodePass;

struct _BoxNodePass {
  CstBoxNodeFunc func;
  SysPointer user_data;
};

SYS_DEFINE_TYPE(CstBoxNode, cst_box_node, CST_TYPE_LAYER_NODE);


SysHNode *_box_node_to_hnode(CstBoxNode *self) {
  if (self == NULL) {
    return NULL;
  }

  return SYS_HNODE(&self->tree_node);
}

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

  SysHNode *p = BOX_NODE_TO_HNODE(parent);
  SysHNode *s = BOX_NODE_TO_HNODE(sibling);
  SysHNode *c = BOX_NODE_TO_HNODE(box_node);

  SysHNode *node = sys_hnode_insert_after(p, s, c);

  return HNODE_TO_BOX_NODE(node);
}

void cst_box_node_append(CstBoxNode *parent, CstBoxNode *box_node) {
  sys_return_if_fail(parent != NULL);
  sys_return_if_fail(box_node != NULL);

  CstBoxNode *last_child = cst_box_node_get_last_child(parent);
  cst_box_node_insert_after(parent, last_child, box_node);

  cst_box_node_set_last_child(parent, box_node);
}

SysBool cst_box_node_has_one_child(CstBoxNode* self) {
  sys_return_val_if_fail(self != NULL, false);

  return sys_hnode_has_one_child(BOX_NODE_TO_HNODE(self));
}

void cst_box_node_repaint_node(CstLayerNode* o, CstLayout* layout) {
  CstRenderNode *rnode = cst_layer_node_get_render_node(o);

  cst_render_node_paint_self(rnode, layout);
}

void cst_box_node_repaint_r(CstBoxNode* self, CstLayout* layout) {
  CstLayerNode *bnode;
  CstLayerNode *lnode = CST_LAYER_NODE(self);
  CstRenderNode *rnode = cst_layer_node_get_render_node(lnode);

  cst_render_node_render_enter(rnode, layout);

  cst_layer_node_repaint_node(lnode, layout);
  bnode = (CstLayerNode *)cst_box_node_children(self);
  if(bnode) {

    cst_render_node_paint_self(rnode, layout);
  }

  bnode = (CstLayerNode *)cst_box_node_next(self);
  if(bnode) {

    cst_layer_node_repaint_node(bnode, layout);
  }

  cst_render_node_render_leave(rnode, layout);
}

void cst_box_node_relayout_r(CstBoxNode* self, CstLayout* layout) {
  sys_return_if_fail(self != NULL);
  CstBoxNode *child, *bnode;
  CstRenderNode* rnode, *rcnode;

  rnode = cst_layer_node_get_render_node(CST_LAYER_NODE(self));
  cst_render_node_render_enter(rnode, layout);

  if(!cst_render_node_need_layout(rnode)) {
    return;
  }

  if(!cst_render_node_is_visible(rnode)) {
    return;
  }

  cst_render_node_layout_self(rnode, layout);


  child = cst_box_node_children(self);
  if (child) {
    rcnode = cst_layer_node_get_render_node(CST_LAYER_NODE(child));

    if (cst_box_node_has_one_child(self)) {

      cst_render_node_inherit(rcnode, rnode, layout);
      cst_box_node_relayout_r(child, layout);
    } else {
      cst_flex_item_layout(CST_FLEX_ITEM(rnode));

      for (bnode = child; bnode; bnode = cst_box_node_next(bnode)) {
        cst_box_node_relayout_r(bnode, layout);
      }
    }
  }

  bnode = cst_box_node_next(self);
  if (bnode) {

    cst_box_node_relayout_r(bnode, layout);
  }

  cst_render_node_render_leave(rnode, layout);
  cst_render_node_set_need_layout(rnode, false);
}

CstLayoutNode *cst_box_node_get_layout_node(CstBoxNode *self) {
  CstLayerNode *lnode = CST_LAYER_NODE(self);
  CstRenderNode *rnode = cst_layer_node_get_render_node(lnode);
  CstLayoutNode *lynode = CST_LAYOUT_NODE(rnode);

  return lynode;
}

void cst_box_node_repaint_root(CstBoxNode *self, CstLayout *layout) {
  FRDraw* draw = cst_layout_get_draw(layout);
  CstLayoutNode *lynode = cst_box_node_get_layout_node(self);
  const FRRect *bound = cst_layout_node_get_bound(lynode);

  fr_draw_fill_bound(draw, bound);
}

SysBool box_node_cb(SysHNode *node, SysPointer user_data) {
  BoxNodePass *pass = user_data;
  CstBoxNode *bnode = HNODE_TO_BOX_NODE(node);

  return pass->func(bnode, pass->user_data);
}

void cst_box_node_bfs_handle(CstBoxNode* self, CstBoxNodeFunc func, SysPointer user_data) {
  BoxNodePass pass = { func, user_data };
  sys_hnode_handle_bfs_r(&self->tree_node, box_node_cb, &pass);
}

void cst_box_node_handle_ft_r(CstBoxNode *self, CstBoxNodeFunc func, SysPointer user_data) {
  sys_return_if_fail(self != NULL);
  BoxNodePass pass = { func, user_data };

  sys_hnode_handle_ft_r(&self->tree_node, box_node_cb, &pass);
}

CstBoxNode* cst_box_node_get_parent(CstBoxNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return HNODE_TO_BOX_NODE(sys_hnode_parent(BOX_NODE_TO_HNODE(self)));
}

SysBool cst_box_node_print(CstBoxNode* self, SysPointer user_data) {
  sys_return_val_if_fail(self != NULL, false);

  CstLayerNode *lnode = CST_LAYER_NODE(self);
  CstLayerNode *pnode = CST_LAYER_NODE(cst_box_node_get_parent(self));

  cst_render_node_print(
    cst_layer_node_get_render_node(lnode),
    pnode ? cst_layer_node_get_render_node(pnode) : NULL);

  return true;
}

void cst_box_node_print_r(CstBoxNode* self, SysPointer user_data) {
  sys_return_if_fail(self != NULL);

  cst_box_node_bfs_handle(self, cst_box_node_print, NULL);
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

static void cst_box_node_construct(CstLayerNode* o, CstLayer *layer, CstNode *node) {

  CST_LAYER_NODE_CLASS(cst_box_node_parent_class)->construct(o, layer, node);
}

CstLayerNode *cst_box_node_new_I(CstNode *node) {
  CstLayerNode *o = cst_box_node_new();
  CstRender *render = cst_render_get_g_render();
  CstLayer *box_layer = cst_render_get_layer_by_type(render, CST_NODE_LAYER_BOX);

  cst_box_node_construct(o, box_layer, node);

  return o;
}

static void cst_box_node_class_init(CstBoxNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstLayerNodeClass *lcls = CST_LAYER_NODE_CLASS(cls);

  ocls->dispose = cst_box_node_dispose;
  lcls->construct = cst_box_node_construct;
}

static void cst_box_node_init(CstBoxNode *self) {
  sys_hnode_init(&self->tree_node);
}

