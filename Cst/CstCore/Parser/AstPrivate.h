#ifndef __AST_PRIVATE_H__
#define __AST_PRIVATE_H__

#include <CstCore/Parser/Ast.h>

SYS_BEGIN_DECLS

struct _Component {
  JNode *property;
  JNode *body;
};

struct _GStyle {
  JNode *property;
  JNode *body;
};

struct _Import {
  SysPtrArray *ids;
  SysChar *path;
};

struct _AstNode {
  AstNodeType type;

  union {
    SysPtrArray *root;
    Import *import;
    Source *source;
    Component *component;
    GStyle *gstyle;
    JNode *jnode;
  } v;
};

struct _Source {
  SysChar* string;
  SysInt startline;
};

struct _JPair {
  SysChar* key;
  JNode* prop;
  JNode* value;
};

struct _JObject {
  JPair** pairs;
  SysInt len;
};

struct _JNode {
  JNodeType type;

  union {
    JNode* v_node;
    JSource* v_source;
    JPointer v_pointer;
    JBool v_bool;
    JDouble v_double;
    JInt v_int;
    JChar* v_string;
    JPair* v_pair;
    JObject* v_object;
    JArray* v_array;
  } v;
};


SYS_END_DECLS

#endif
