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

  void (*parse) (CstNodeBuilder *self, AstParser *c, JNode *jnode);
  void (*build) (CstNodeBuilder *self, AstParser *c, CstNode *o);
};

SysType cst_node_builder_get_type(void);
CstNodeBuilder *cst_node_builder_new(void);

const SysChar* cst_node_builder_get_value(CstNodeBuilder *self);

void cst_node_builder_set_label(CstNodeBuilder *self, const SysChar *v_label);
void cst_node_builder_set_id(CstNodeBuilder *self, const SysChar *v_id);
void cst_node_builder_set_v_value(CstNodeBuilder *self, const SysChar *v_value);
SysBool cst_node_builder_set_layer(CstNodeBuilder *self, SysInt v_layer);

void cst_node_builder_add_awatch(CstNodeBuilder *self, FRAWatch* map);
SysList * cst_node_builder_get_awatch_list(CstNodeBuilder *self);

void cst_node_builder_add_nodemap(CstNodeBuilder *self, CstNodeMap* map);
SysList * cst_node_builder_get_nodemap_list(CstNodeBuilder *self);

SysChar* cst_node_builder_extract_index(const SysChar* str, SysInt slen);

void cst_node_builder_parse(CstNodeBuilder *self, AstParser *c, JNode *jnode);
void cst_node_builder_build(CstNodeBuilder *self, AstParser *c, CstNode *node);

SysBool cst_node_builder_parse_value_bind(CstNodeBuilder *self, AstParser *c, const SysChar *key, const SysChar *expr_str);
SysBool cst_node_builder_parse_action(CstNodeBuilder *self, AstParser *c, const SysChar *watch_name, const SysChar *func_name);
SysBool cst_node_builder_parse_layer_name(CstNodeBuilder *self, const SysChar *pstr);

void cst_node_builder_set_v_css_list(CstNodeBuilder *self, SysPtrArray * v_css_list);
SysPtrArray * cst_node_builder_get_v_css_list(CstNodeBuilder *self);

SYS_END_DECLS

#endif

