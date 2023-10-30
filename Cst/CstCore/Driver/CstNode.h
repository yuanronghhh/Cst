#ifndef __CST_NODE_H__
#define __CST_NODE_H__

#include <CstCore/Driver/CstNodeBuilder.h>


SYS_BEGIN_DECLS

#define CST_TYPE_NODE (cst_node_get_type())
#define CST_NODE(o) ((CstNode* )sys_object_cast_check(o, CST_TYPE_NODE))
#define CST_NODE_CLASS(o) ((CstNodeClass *)sys_class_cast_check(o, CST_TYPE_NODE))
#define CST_NODE_GET_CLASS(o) sys_instance_get_class(o, CstNodeClass)

struct _CstNode {
  FRNode unowned;

  /* <private> */
  CstNode     *last_child;
  SysChar     *name;
  SysChar     *id;
  SysType     rnode_type;

  /* builder */
  CstModule* v_module;
  CstComponent* v_component;
  CstNode* v_pnode;

  /* property */
  SysList *v_awatch_list;
  SysList *v_nodemap_list;
  SysPtrArray *v_css_list;

  const SysChar* v_id;
  const SysChar *v_tag;

  CstLayer *v_layer;
  SysChar *v_value;
  SysChar *v_label;
  SysInt  v_z_index;
};

struct _CstNodeClass {
  FRNodeClass parent;

  void (*construct) (CstNode* v_node, CstNodeBuilder* builder);
  CstLayerNode* (*realize) (CstNode* self, CstLayerNode *v_parent, CstComNode *com_node);
};

CstNode* cst_node_new(void);
CstNode* cst_node_new_I(CstNodeBuilder *builder);
SysType cst_node_get_type(void);

CST_NODE_PROP_ENUM cst_node_prop_get_by_name(const SysChar *name);

void cst_node_set_name(CstNode *node, const SysChar *name);
const SysChar *cst_node_get_name(CstNode *node);

void cst_node_set_id(CstNode *node, const SysChar *id);
const SysChar *cst_node_get_id(CstNode *node);

CST_NODE_LAYER_ENUM cst_node_layer_by_name(const SysChar* name);

void cst_node_construct(CstNode *self, CstNodeBuilder *builder);
CstLayerNode* cst_node_realize(CstNode *self, CstLayerNode *v_parent, CstComNode *com_node);
CstLayerNode* cst_node_realize_r(CstNode *self, CstLayerNode *v_parent, CstComNode *com_node);

void cst_node_set_rnode_type(CstNode *self, SysType rnode_type);
SysType cst_node_get_rnode_type(CstNode *self);

CstRenderNode *cst_node_new_render_node(CstNode* self);
void cst_node_unlink_node_r(CstNode *self);

/* props */
void cst_node_set_v_module(CstNode *self, CstModule * v_module);
CstModule * cst_node_get_v_module(CstNode *self);

void cst_node_set_v_component(CstNode *self, CstComponent * v_component);
CstComponent * cst_node_get_v_component(CstNode *self);

void cst_node_set_v_pnode(CstNode *self, CstNode * v_pnode);
CstNode * cst_node_get_v_pnode(CstNode *self);

void cst_node_set_v_awatch_list(CstNode *self, SysList * v_awatch_list);
SysList * cst_node_get_v_awatch_list(CstNode *self);

void cst_node_set_v_nodemap_list(CstNode *self, SysList * v_nodemap_list);
SysList * cst_node_get_v_nodemap_list(CstNode *self);

void cst_node_set_v_css_list(CstNode *self, SysPtrArray * v_css_list);
SysPtrArray * cst_node_get_v_css_list(CstNode *self);

void cst_node_set_v_id(CstNode *self, const SysChar * v_id);
const SysChar * cst_node_get_v_id(CstNode *self);

void cst_node_set_v_value(CstNode *self, SysChar * v_value);
SysChar * cst_node_get_v_value(CstNode *self);

void cst_node_set_v_label(CstNode *self, SysChar * v_label);
SysChar * cst_node_get_v_label(CstNode *self);

void cst_node_set_v_z_index(CstNode *self, SysInt v_z_index);
SysInt cst_node_get_v_z_index(CstNode *self);

void cst_node_set_v_layer(CstNode *self, CstLayer* v_layer);
CstLayer* cst_node_get_v_layer(CstNode *self);

SYS_END_DECLS

#endif
