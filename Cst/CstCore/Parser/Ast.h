#ifndef __AST_H__
#define __AST_H__

#include <CstCore/Parser/AstJNode.h>
#include <CstCore/Parser/CstParser.h>

SYS_BEGIN_DECLS

typedef enum _AstNodeType {
  AstRoot,
  AstImport,
  AstSource,
  AstComponent,
  AstGStyle,

  AstJsonNode,
  AstInvalid
} AstNodeType;

typedef SysBool (*AstNodeFunc) (AstNode *node, SysPointer user_data);

AstNode* ast_for_root(Parser *ps, JArray *array);
AstNode* ast_for_import(Parser *ps, SysPtrArray *array, SysChar *path);
AstNode* ast_for_component(Parser *ps, JNode* prop_node, JNode* body_node);
AstNode* ast_for_gstyle(Parser *ps, JNode* prop_node, JNode *body_node);
AstNode* ast_for_source(Parser *ps, const SysChar *str, SysInt rows);

SysBool ast_print_node(AstNode *node, SysPointer user_data);
SysBool ast_print_jnode(JNode *node, SysPointer user_data);
void ast_print_visitor(AstNode *node, SysPointer user_data);

void ast_iter_narray(SysPtrArray *array, AstNodeFunc func, SysPointer user_data);

void ast_import_free(Import* imp);
void ast_source_free(Source* source);
void ast_node_free(AstNode* node);
void ast_component_free(Component* component);
void ast_gstyle_free(GStyle* gstyle);

/* user api */
GStyle *ast_root_get_gstyle(AstNode *root);
void ast_gstyle_parse(GStyle * gstyle, CstCssEnv * gcss_env, const SysChar * path);
void ast_node_parse(JNode * jnode, CstComponent * component, CstNodeProps * props);
CstCssGroup * ast_css_group_new_with_jpair(CstCssEnv *env, JPair * pair, SysBool key_lookup);
void ast_module_parse(AstNode * root, CstModule * self);
SysBool ast_component_property_parse(JNode * jnode, CstComponentProps * v_props);
void ast_import_parse(Parser * ps, AstNode * node);

SysInt ast_css_value_parse(JNode * jnode, CstCssValue * value);
SysInt ast_css_value_width_parse(JNode * v, CstCssValue * value);
SysInt ast_css_value_height_parse(JNode * v, CstCssValue * value);
SysInt ast_css_value_color_parse(JNode * v, CstCssValue * value);
void ast_codegen_node_parse(CodeGen * cg, AstNode * node);
SysValue * ast_jnode_new_value(JNode * jnode);

SYS_END_DECLS

#endif

