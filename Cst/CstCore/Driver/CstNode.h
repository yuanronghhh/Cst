#ifndef __CST_NODE_H__
#define __CST_NODE_H__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_NODE (cst_node_get_type())
#define CST_NODE(o) ((CstNode* )sys_object_cast_check(o, CST_TYPE_NODE))
#define CST_NODE_CLASS(o) ((CstNodeClass *)sys_class_cast_check(o, CST_TYPE_NODE))
#define CST_NODE_GET_CLASS(o) sys_instance_get_class(o, CstNodeClass)

struct _CstNode {
  FRNode      unowned;
  /* <private> */
  SysChar     *name;
  SysChar     *id;
  SysType     rnode_type;

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

  CstLayerNode* (*realize) (CstNode* self, CstLayerNode *v_parent, CstComNode *com_node);
  void (*construct) (CstNode* self, CstNodeBuilder *builder);
};

CstNode* cst_node_new(void);
SysType cst_node_get_type(void);

CST_NODE_PROP_ENUM cst_node_prop_get_by_name(const SysChar *name);

void cst_node_set_name(CstNode *node, const SysChar *name);
const SysChar *cst_node_get_name(CstNode *node);

void cst_node_set_id(CstNode *node, const SysChar *id);
const SysChar *cst_node_get_id(CstNode *node);

CstNode* cst_node_new_layout_node(CstModule* v_module);
CstNode *cst_node_new_body(void);

void cst_node_print_node(CstNode* node, SysPointer user_data);
void cst_node_print_r(CstNode* node, SysPointer user_data);

CstLayerNode* cst_node_realize(CstNode *self, CstLayerNode *v_parent, CstComNode *com_node);
CstLayerNode* cst_node_realize_r(CstNode *self, CstLayerNode *v_parent, CstComNode *com_node);

void cst_node_set_rnode_type(CstNode *self, SysType rnode_type);
SysType cst_node_get_rnode_type(CstNode *self);

CstRenderNode *cst_node_new_render_node(CstNode* self);
void cst_node_unlink_node_r(CstNode *self);

CstNode *cst_node_get_body_node(void);
void cst_node_setup(void);
void cst_node_teardown(void);

void cst_node_construct (CstNode* self, CstNodeBuilder *builder);

/* props */
void cst_node_add_awatch(CstNode *self, FRAWatch* o);
void cst_node_add_nodemap(CstNode *self, CstNodeMap* o);

void cst_node_set_v_css_list(CstNode *self, SysPtrArray * v_css_list);
SysPtrArray * cst_node_get_v_css_list(CstNode *self);

void cst_node_set_v_id(CstNode *self, const SysChar * v_id);
const SysChar * cst_node_get_v_id(CstNode *self);

void cst_node_set_v_value(CstNode *self, const SysChar * v_value);
SysChar * cst_node_get_v_value(CstNode *self);

void cst_node_set_v_label(CstNode *self, SysChar * v_label);
SysChar * cst_node_get_v_label(CstNode *self);

void cst_node_set_v_z_index(CstNode *self, SysInt v_z_index);
SysInt cst_node_get_v_z_index(CstNode *self);

void cst_node_set_v_layer(CstNode *self, CstLayer* v_layer);
CstLayer* cst_node_get_v_layer(CstNode *self);

SYS_END_DECLS

#endif
