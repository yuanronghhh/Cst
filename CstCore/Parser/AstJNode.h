#ifndef __AST_JNODE_H__
#define __AST_JNODE_H__

#include <CstCore/Parser/CstParserCommon.h>

SYS_BEGIN_DECLS

SysBool ast_print_jnode(JNode *node, SysPointer user_data);
JNode * ast_jnode_clone_r(JNode * jnode);
SysInt ast_jnode_get_type(JNode* jnode);
JNode* ast_jpair_value_value(JNode *node);

void ast_iter_jarray(JNode *node, AstJNodeFunc func, SysPointer user_data);
#define ast_iter_jobject(node, func, user_data) ast_iter_jarray(node, func, user_data)

JNode* ast_for_jbool(bool bvalue);
JNode* ast_for_jnull(void);
JNode* ast_for_jdouble(const SysDouble d);
JNode* ast_for_jint(const SysInt i);

const SysChar *ast_jnode_get_string(JNode *jnode);
JNode* ast_for_jstring(SysChar *str);

JNode* ast_for_jproperty(JArray *array);
JNode* ast_for_jobject(JArray *array);
JNode* ast_for_jarray(JArray *array);
JNode* ast_for_jpair(SysChar *key, JNode *prop, JNode *value);
JNode* ast_for_jnode(JNode *cnode);
JNode* ast_for_jsource(SysChar *str);
JSource *ast_jsource_new(SysChar * str);

void ast_jpair_free(JPair *pair);
void ast_jnode_free(JNode *node);

JNode * ast_jpair_get_by_key(JNode * jnode, SysChar * key);
JNode* ast_jnode_jnode(JNode *node);
JNode* ast_get_jobject_value(JNode *array_node, SysChar *key);
JNode * ast_jpair_value(JNode * jnode);
JPair* ast_jnode_jpair(JNode* jnode);
SysValue *ast_jnode_to_value(JNode *nnode);


SYS_END_DECLS

#endif
