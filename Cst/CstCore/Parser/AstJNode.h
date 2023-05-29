#ifndef __AST_JNODE_H__
#define __AST_JNODE_H__

#include <CstCore/Parser/CstParserCommon.h>

SYS_BEGIN_DECLS

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

struct _Source {
  SysChar *string;
  SysInt startline;
};

struct _JPair {
  SysChar *key;
  JNode *prop;
  JNode *value;
};

struct _JObject {
  JPair **pairs;
  SysInt len;
};

struct _JNode {
  JNodeType type;

  union {
    JNode *v_node;
    JSource *v_source;
    JPointer v_pointer;
    JBool v_bool;
    JDouble v_double;
    JInt v_int;
    JChar* v_string;
    JPair *v_pair;
    JObject *v_object;
    JArray *v_array;
  } v;
};

typedef SysBool (*AstJNodeFunc) (JNode *jnode, SysPointer user_data);
typedef SysInt (*AstJObjectValueFunc) (SysChar *key, JNodeType nodeType, SysPointer value);

SysChar * ast_jnode_extract_index(const SysChar * index_str, SysInt len);
JNode * ast_jnode_index(JNode * o, const SysChar * index_str);
JNode * ast_jnode_clone_r(JNode * jnode);
SysBool ast_print_jnode(JNode *node, SysPointer user_data);

void ast_iter_jarray(JNode *node, AstJNodeFunc func, SysPointer user_data);
#define ast_iter_jobject(node, func, user_data) ast_iter_jarray(node, func, user_data)

JNode* ast_for_jbool(Parser *ps, bool bvalue);
JNode* ast_for_jnull(Parser *ps);
JNode* ast_for_jdouble(Parser *ps, const SysDouble d);
JNode* ast_for_jint(Parser *ps, const SysInt i);
JNode* ast_for_jstring(Parser *ps, SysChar *str);
JNode* ast_for_jproperty(Parser *ps, JArray *array);
JNode* ast_for_jobject(Parser *ps, JArray *array);
JNode* ast_for_jarray(Parser *ps, JArray *array);
JNode* ast_for_jpair(Parser *ps, SysChar *key, JNode *prop, JNode *value);
JNode* ast_for_jnode(Parser *ps, JNode *cnode);
JNode* ast_for_jsource(Parser *ps, SysChar *str);

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

