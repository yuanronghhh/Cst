#ifndef __CST_NODE_BUILDER_H__
#define __CST_NODE_BUILDER_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


#define CST_TYPE_NODE_BUILDER (cst_node_builder_get_type())
#define CST_NODE_BUILDER(o) ((CstNodeBuilder* )sys_object_cast_check(o, CST_TYPE_NODE_BUILDER))
#define CST_NODE_BUILDER_CLASS(o) ((CstNodeBuilderClass *)sys_class_cast_check(o, CST_TYPE_NODE_BUILDER))
#define CST_NODE_BUILDER_GET_CLASS(o) sys_instance_get_class(o, CstNodeBuilderClass)


struct _CstNodeBuilder {
  SysObject parent;

  /* < private > */
  CstModule* v_module;
  CstComponent* v_component;
  CstNode* v_pnode;

  /* property */
  SysList *v_awatches;
  SysList *v_node_maps;
  const SysChar* v_id;
  const SysChar *v_tag;
  SysChar **v_base;
  SysInt  v_base_len;
  SysInt v_position;
  SysChar *v_value;
  SysChar *v_label;
  SysInt  v_z_index;
};

struct _CstNodeBuilderClass {
  SysObjectClass parent;

  void (*construct) (CstNodeBuilder *o, CstModule* v_module, CstComponent* v_component, CstNode* v_pnode);
};

SysType cst_node_builder_get_type(void);
CstNodeBuilder *cst_node_builder_new(void);
CstNodeBuilder *cst_node_builder_new_I(CstModule* v_module, CstComponent* v_component, CstNode* v_pnode);
CstNodeBuilder *cst_node_builder_new_simple(CstModule* v_module, CstNode* v_pnode);
void cst_node_builder_build(CstNodeBuilder *self, CstNode *v_node);
CstModule* cst_node_builder_get_module(CstNodeBuilder *self);
CstNode* cst_node_builder_get_pnode(CstNodeBuilder *self);
CstComponent* cst_node_builder_get_component(CstNodeBuilder *self);
const SysChar* cst_node_builder_get_value(CstNodeBuilder *self);

void cst_node_builder_set_label(CstNodeBuilder *self, SysChar *v_label);
void cst_node_builder_set_base(CstNodeBuilder *self, SysChar *v_base[], SysUInt v_len);
void cst_node_builder_set_id(CstNodeBuilder *self, SysChar *v_id);
void cst_node_builder_set_value(CstNodeBuilder *self, SysChar *v_value);
void cst_node_builder_set_position(CstNodeBuilder *self, SysInt v_position);
void cst_node_builder_add_node_maps(CstNodeBuilder *self, CstNodeMap* map);
void cst_node_builder_add_awatches(CstNodeBuilder *self, FRAWatch* map);

SYS_END_DECLS

#endif

