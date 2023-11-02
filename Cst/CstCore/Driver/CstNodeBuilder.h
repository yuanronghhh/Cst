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
  CstContext *c;
  CstNode* v_pnode;
  CstNode *v_com_node;

  /* property */
  SysList *v_awatch_list;
  SysList *v_nodemap_list;
  SysPtrArray *v_css_list;

  const SysChar* v_id;
  const SysChar *v_tag;

  SysInt v_layer;
  SysChar *v_value;
  SysChar *v_label;
  SysInt  v_z_index;
};

struct _CstNodeBuilderClass {
  SysObjectClass parent;

  void (*parse) (CstNodeBuilder *self, CstContext *c, JNode *jnode);
  void (*build) (CstNodeBuilder *self, CstContext *c, CstNode *o);
};

SysType cst_node_builder_get_type(void);
CstNodeBuilder *cst_node_builder_new(void);
CstNodeBuilder *cst_node_builder_new_I(CstContext *c, CstNode* v_pnode);

CstNode* cst_node_builder_get_pnode(CstNodeBuilder *self);
const SysChar* cst_node_builder_get_value(CstNodeBuilder *self);

void cst_node_builder_set_label(CstNodeBuilder *self, const SysChar *v_label);
void cst_node_builder_set_id(CstNodeBuilder *self, const SysChar *v_id);
void cst_node_builder_set_v_value(CstNodeBuilder *self, const SysChar *v_value);
SysBool cst_node_builder_set_layer(CstNodeBuilder *self, SysInt v_layer);

void cst_node_builder_set_v_module(CstNodeBuilder *self, CstModule* v_module);
CstModule* cst_node_builder_get_v_module(CstNodeBuilder *self);

void cst_node_builder_set_v_component(CstNodeBuilder *self, CstComponent* v_component);
CstComponent* cst_node_builder_get_v_component(CstNodeBuilder *self);

void cst_node_builder_add_awatch(CstNodeBuilder *self, FRAWatch* map);
SysList * cst_node_builder_get_awatch_list(CstNodeBuilder *self);

void cst_node_builder_add_nodemap(CstNodeBuilder *self, CstNodeMap* map);
SysList * cst_node_builder_get_nodemap_list(CstNodeBuilder *self);

SysChar* cst_node_builder_extract_index(const SysChar* str, SysInt slen);

void cst_node_builder_parse(CstNodeBuilder *self, CstContext *c, JNode *jnode);
void cst_node_builder_build(CstNodeBuilder *self, CstContext *c, CstNode *node);

SysBool cst_node_builder_parse_value_bind(CstNodeBuilder *self, CstContext *c, const SysChar *key, const SysChar *expr_str);
SysBool cst_node_builder_parse_action(CstNodeBuilder *self, CstContext *c, const SysChar *watch_name, const SysChar *func_name);
SysBool cst_node_builder_parse_layer_name(CstNodeBuilder *self, const SysChar *pstr);

SYS_END_DECLS

#endif

