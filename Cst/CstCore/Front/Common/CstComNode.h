#ifndef __CST_COM_NODE_H__
#define __CST_COM_NODE_H__

#include <CstCore/Driver/CstNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_COM_NODE (cst_com_node_get_type())
#define CST_COM_NODE(o) ((CstComNode* )sys_object_cast_check(o, CST_TYPE_COM_NODE))
#define CST_COM_NODE_CLASS(o) ((CstComNodeClass *)sys_class_cast_check(o, CST_TYPE_COM_NODE))
#define CST_COM_NODE_GET_CLASS(o) sys_instance_get_class(o, CstComNodeClass)


struct _CstComNode {
  CstNode parent;

  /* <private> */
  CstComponent *component;
  SysHashTable *values_ht;
};

struct _CstComNodeClass {
  CstNodeClass parent;
};

SysType cst_com_node_get_type(void);
CstNode* cst_com_node_new(void);
CstNode* cst_com_node_new_I(CstComponent *comp);

CstNodeMap* cst_com_node_get_node_map(CstComNode* self, const SysChar *key);
void cst_com_node_set_node_map(CstComNode* self, CstNodeMap *map);

CstNodeMapFunc cst_com_node_get_func(SysType node_type, SysInt prop_type, SysInt data_type);

void cst_com_node_set_component(CstComNode *self, CstComponent * component);
CstComponent * cst_com_node_get_component(CstComNode *self);

void cst_com_node_set_values_ht(CstComNode *self, SysHashTable * values_ht);
SysHashTable * cst_com_node_get_values_ht(CstComNode *self);

SYS_END_DECLS

#endif

