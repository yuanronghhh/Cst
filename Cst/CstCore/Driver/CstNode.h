#ifndef __CST_NODE_H__
#define __CST_NODE_H__

#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstNodeBuilder.h>


SYS_BEGIN_DECLS

#define CST_TYPE_NODE (cst_node_get_type())
#define CST_NODE(o) ((CstNode* )sys_object_cast_check(o, CST_TYPE_NODE))
#define CST_NODE_CLASS(o) ((CstNodeClass *)sys_class_cast_check(o, CST_TYPE_NODE))
#define CST_NODE_GET_CLASS(o) sys_instance_get_class(o, CstNodeClass)

struct _CstNode {
  SysObject unowned;

  CstNode   *next;
  CstNode   *prev;
  CstNode   *parent;
  CstNode   *children;

  /* <private> */
  CstNode     *last_child;
  SysChar     *name;
  SysChar     *id;

  SysType v_default_rctx;
  CstNodeBuilder *builder;
};

struct _CstNodeClass {
  SysObjectClass parent;

  void (*construct) (CstNode* v_node, CstNodeBuilder* builder);
  CstRenderNode* (*realize) (CstNode* self, CstRenderNode* prnode, CstLayout *layout);
};

CstNode* cst_node_new(void);
CstNode* cst_node_new_I(CstNodeBuilder* builder);
SysType cst_node_get_type(void);

CstNode* cst_node_parent(CstNode *node);
CST_NODE_PROP_ENUM cst_node_prop_get_by_name(const SysChar *name);

void cst_node_unlink_node_r(CstNode *node);

CstNode* cst_node_insert_after(CstNode *parent, CstNode *sibling, CstNode *node);
void cst_node_append(CstNode *parent, CstNode *node);

void cst_node_print_node_r(CstNode* node);
void cst_node_print_node(CstNode *node);

void cst_node_set_name(CstNode *node, const SysChar *name);
const SysChar *cst_node_get_name(CstNode *node);

void cst_node_set_id(CstNode *node, const SysChar *id);
const SysChar *cst_node_get_id(CstNode *node);

CST_NODE_POSITION_ENUM cst_node_position_by_name(const SysChar* name);

CstRenderNode* cst_node_realize(CstNode* self, CstRenderNode* prnode, CstLayout *layout);
CstNode *cst_node_get_last_child(CstNode *node);
void cst_node_set_last_child(CstNode *node, CstNode *last_child);

CstRenderNode* cst_node_realize_r(CstNode *self, CstRenderNode *prnode, CstLayout *layout);
CstRenderNode* cst_node_realize_self(CstRenderNode* prnode, CstNode* self, CstLayout *layout);

void cst_node_construct(CstNode *self, CstNodeBuilder *builder);

void cst_node_set_builder(CstNode *self, CstNodeBuilder * builder);
CstNodeBuilder * cst_node_get_builder(CstNode *self);

CstRenderContext * cst_node_new_default_rctx(CstNode *self);

SYS_END_DECLS

#endif
