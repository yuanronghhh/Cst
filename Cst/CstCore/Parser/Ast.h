#ifndef __AST_H__
#define __AST_H__

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

typedef enum _AstJNodeType {
  AstJBool = 1,
  AstJString,
  AstJArray,
  AstJObject,
  AstJNull,
  AstJSource,

  AstJInt,
  AstJDouble,
  AstJProperty,
  AstJPair,
  AstJNode,
  AstJPointer,

  AstJInvalid
} JNodeType;

typedef SysBool (*AstJNodeFunc) (JNode *jnode, SysPointer user_data);
typedef SysInt (*AstJObjectValueFunc) (SysChar *key, JNodeType nodeType, SysPointer value);
typedef SysBool (*AstNodeFunc) (AstNode *node, SysPointer user_data);

AstNode* ast_for_root(CstParser *ps, JArray *array);
AstNode* ast_for_import(CstParser *ps, SysPtrArray *array, SysChar *path);
AstNode* ast_for_component(CstParser *ps, JNode* prop_node, JNode* body_node);
AstNode* ast_for_gstyle(CstParser *ps, JNode* prop_node, JNode *body_node);
AstNode* ast_for_source(CstParser *ps, const SysChar *str, SysInt rows);

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
void ast_node_parse(JNode * jnode, CstComponent * component, CstNodeBuilder *builder);
CstCssGroup * ast_css_group_new_with_jpair(CstCssEnv *env, JPair * pair, SysBool key_lookup);
void ast_module_parse(AstNode * root, CstModule * self);
SysBool ast_component_property_parse(JNode *jnode, CstComponentBuilder *v_comp_bd);
void ast_import_parse(CstParser * ps, AstNode * node);

SysInt ast_css_value_parse(JNode *jnode, SysChar *key, CstCssValue * value);
SysInt ast_css_value_width_parse(JNode * v, CstCssValue * value);
SysInt ast_css_value_height_parse(JNode * v, CstCssValue * value);
SysInt ast_css_value_color_parse(JNode * v, CstCssValue * value);
SysValue * ast_jnode_new_value(JNode * jnode);

SysChar * ast_jnode_extract_index(const SysChar * index_str, SysInt len);
JNode * ast_jnode_index(JNode * o, const SysChar * index_str);
JNode * ast_jnode_clone_r(JNode * jnode);
SysBool ast_print_jnode(JNode *node, SysPointer user_data);

void ast_iter_jarray(JNode *node, AstJNodeFunc func, SysPointer user_data);
#define ast_iter_jobject(node, func, user_data) ast_iter_jarray(node, func, user_data)

JNode* ast_for_jbool(CstParser *ps, bool bvalue);
JNode* ast_for_jnull(CstParser *ps);
JNode* ast_for_jdouble(CstParser *ps, const SysDouble d);
JNode* ast_for_jint(CstParser *ps, const SysInt i);
JNode* ast_for_jstring(CstParser *ps, SysChar *str);
JNode* ast_for_jproperty(CstParser *ps, JArray *array);
JNode* ast_for_jobject(CstParser *ps, JArray *array);
JNode* ast_for_jarray(CstParser *ps, JArray *array);
JNode* ast_for_jpair(CstParser *ps, SysChar *key, JNode *prop, JNode *value);
JNode* ast_for_jnode(CstParser *ps, JNode *cnode);
JNode* ast_for_jsource(CstParser *ps, SysChar *str);

void ast_jpair_free(JPair *pair);
void ast_jnode_free(JNode *node);

JNode * ast_jpair_get_by_key(JNode * jnode, SysChar * key);
JNode* ast_jnode_jnode(JNode *node);
JNode* ast_get_jobject_value(JNode *array_node, SysChar *key);
JNode * ast_jpair_value(JNode * jnode);
SysBool ast_jnode_is_type(JNode *jnode, JNodeType type);
JSource *ast_jsource_new(SysChar * str);


SYS_END_DECLS

#endif

