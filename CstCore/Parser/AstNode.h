#ifndef __AST_NODE_H__
#define __AST_NODE_H__

#include <CstCore/Parser/CstParserCommon.h>

SYS_BEGIN_DECLS

SysBool ast_print_node(AstNode *node, SysPointer user_data);
void ast_print_visitor(AstNode *node, SysPointer user_data);
void ast_iter_narray(SysPtrArray *array, AstNodeFunc func, SysPointer user_data);

AstNode* ast_for_root(JArray *array);
void ast_node_free(AstNode* node);

AstNode* ast_for_import(SysPtrArray *array, SysChar *path);
void ast_import_free(Import* imp);

AstNode* ast_for_gstyle(JNode* prop_node, JNode *body_node);
void ast_gstyle_free(GStyle* gstyle);

AstNode* ast_for_component(JNode* prop_node, JNode *body_node);
void ast_component_free(Component* component);

AstNode* ast_for_source(const SysChar *str, SysInt rows);
void ast_source_free(Source* source);

AstNode *ast_root_get_gstyle(AstNode *root);

SYS_END_DECLS

#endif

