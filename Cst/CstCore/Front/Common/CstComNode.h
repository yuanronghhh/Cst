#ifndef __CST_COM_NODE_H__
#define __CST_COM_NODE_H__

#include <CstCore/Front/Common/CstNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_COM_NODE _CST_TYPE_COM_NODE
#define CST_COM_NODE(o) _CST_COM_NODE(o)
#define CST_COM_NODE_CLASS(o) _CST_COM_NODE_CLASS(o)
#define CST_COM_NODE_GET_CLASS(o) _CST_COM_NODE_GET_CLASS(o)

SysType cst_com_node_get_type(void);
CstNode* cst_com_node_new(void);
CstNode * cst_com_node_new_with_component(CstComponent * comp);
CstComponent * cst_com_node_get_component(CstComNode * self);

SysValue * cst_com_node_get_value(CstComNode * self, const SysChar *key);
void com_node_set_value(CstComNode * self, const SysChar *key, SysValue * value);
CstNodeMapFunc cst_com_node_get_func(SysType node_type, SysInt prop_type, SysInt data_type);

SYS_END_DECLS

#endif

