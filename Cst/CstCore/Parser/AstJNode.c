#include <CstCore/Parser/AstJNode.h>
#include <CstCore/Parser/CstParser.h>

typedef struct _AstKeyNode AstKeyNode;

struct _AstKeyNode {
  SysChar *key;
  SysPointer value;
};

JNode *ast_jpair_get_by_key(JNode *jnode, SysChar *key) {
  sys_return_val_if_fail(key != NULL, NULL);

  JPair *pair = jnode->v.v_pair;

  if (sys_str_equal(pair->key, key)) {
    return pair->value;
  }

  return NULL;
}

JNode *ast_jnode_jnode(JNode *node) {
  sys_return_val_if_fail(node != NULL, NULL);
  sys_return_val_if_fail(node->type == AstJNode, NULL);

  return node->v.v_node;
}

static bool jkey_equal(JNode *node, SysPointer user_data) {
  AstKeyNode *knode = user_data;
  JPair *pair = node->v.v_pair;

  if (sys_str_equal(pair->key, knode->key)) {
    knode->value = pair->value->v.v_node;
    return false;
  }

  return true;
}

SysBool ast_jnode_is_type(JNode *jnode, JNodeType type) {
  return jnode != NULL && jnode->type == type;
}

JNode *ast_get_jobject_value(JNode *onode, SysChar *key) {
  sys_return_val_if_fail((onode->type == AstJProperty) || (onode->type == AstJObject), NULL);

  AstKeyNode kn = { key, NULL };

  ast_iter_jobject(onode, jkey_equal, &kn);

  return kn.value;
}

JNode *ast_jpair_value(JNode *jnode) {
  sys_return_val_if_fail(jnode->type == AstJPair, NULL);

  return jnode->v.v_pair->value;
}

/* JNode */
SysChar *ast_jnode_extract_index(const SysChar *str, SysInt slen) {
  SysChar *sp;
  SysChar *nsp;

  if (slen < 4) {
    return NULL;
  }

  if (*str != '{' || *(str + 1) != '{') {
    return NULL;
  }

  if (*(str + slen - 1) != '}' || *(str + slen - 2) != '}') {
    return NULL;
  }

  nsp = sys_new0_N(SysChar, slen - 3);
  sp = nsp;

  str += 2;
  while (*str) {
    if (*str == '|' || *str == '}') {
      break;
    }

    if (*str == ' ') {
      str++;
      continue;
    }

    *sp++ = *str++;
  }
  *sp = '\0';

  return nsp;
}

JNode *ast_jnode_index(JNode *o, const SysChar *index_str) {
  sys_return_val_if_fail(o != NULL, NULL);

  // TODO: find info
  JNode *rs = ast_jnode_jnode(o->v.v_array->pdata[1]);

  return rs;
}

JNode *ast_jnode_clone_r(JNode *jnode) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(jnode->type > 0 && jnode->type <= AstJInvalid, false);

  JNode *nnode = NULL;

  switch (jnode->type) {
    case AstJInt:
      nnode = sys_new0_N(JNode, 1);
      nnode->v.v_int = jnode->v.v_int;
      break;
    case AstJDouble:
      nnode->v.v_double = jnode->v.v_double;
      break;
    case AstJNull:
      nnode->v.v_int = 0;
      break;
    case AstJBool:
      nnode->v.v_bool = jnode->v.v_bool;
      break;
    case AstJSource:
      nnode->v.v_source = ast_jsource_new(jnode->v.v_source->string);
      break;
    case AstJString:
      nnode->v.v_string = jnode->v.v_string;
      break;
    case AstJProperty:
    case AstJObject:
    case AstJArray:
      break;
    case AstJNode:
      nnode = ast_jnode_clone_r(jnode);
      break;
    case AstJPair:
      sys_debug_N("[pairkey] %s", jnode->v.v_pair->key);
      if (jnode->v.v_pair->prop) {
      }
      break;
    case AstJInvalid:
      sys_warning_N("%s", "jnode invalid type");
      break;
    default:
      break;
  }

  return nnode;
}

SysBool ast_print_jnode(JNode *node, SysPointer user_data) {
  sys_return_val_if_fail(node != NULL, false);
  sys_return_val_if_fail(node->type > 0 && node->type <= AstJInvalid, false);

  switch (node->type) {
    case AstJInt:
      sys_debug_N("[int] %ld", node->v.v_int);
      break;
    case AstJDouble:
      sys_debug_N("[double] %lf", node->v.v_double);
      break;
    case AstJNull:
      sys_debug_N("[null] %s", node->v.v_node);
      break;
    case AstJBool:
      sys_debug_N("[bool] %d", node->v.v_bool);
      break;
    case AstJSource:
      sys_debug_N("[v_source] %s", node->v.v_source->string);
      break;
    case AstJString:
      sys_debug_N("[string] %s", node->v.v_string);
      break;
    case AstJProperty:
    case AstJObject:
    case AstJArray:
      ast_iter_jarray(node, ast_print_jnode, user_data);
      break;
    case AstJNode:
      ast_print_jnode(node->v.v_node, user_data);
      break;
    case AstJPair:
      sys_debug_N("[pairkey] %s", node->v.v_pair->key);
      if (node->v.v_pair->prop) {
        ast_print_jnode(node->v.v_pair->prop, user_data);
      }
      ast_print_jnode(node->v.v_pair->value, user_data);
      break;
    case AstJInvalid:
      sys_warning_N("%s", "jnode invalid type");
      break;
    default:
      break;
  }

  return true;
}

void ast_iter_jarray(JNode *jnode, AstJNodeFunc func, SysPointer user_data) {
  sys_return_if_fail((jnode->type == AstJObject) || (jnode->type == AstJArray) || (jnode->type == AstJProperty));
  sys_return_if_fail(func != NULL);

  JArray *array = jnode->v.v_array;
  if (array == NULL) { return; }

  for (SysInt i = 0; i < (SysInt)array->len; i++) {
    JNode *prop = array->pdata[i];

    if (!func(prop, user_data)) {
      break;
    }
  }
}

JSource *ast_jsource_new(SysChar *str) {
  JSource *source = sys_new0_N(JSource, 1);

  source->string = str;

  return source;
}

JNode* ast_for_jsource(Parser *ps, SysChar *str) {
  sys_return_val_if_fail(str != NULL, NULL);

  JNode *node = sys_new0_N(JNode, 1);
  JSource *source = ast_jsource_new(str);

  node->type = AstJSource;
  node->v.v_source = source;

  return node;
}

JNode *ast_for_jstring(Parser *ps, SysChar *str) {
  JNode *node = sys_new0_N(JNode, 1);

  node->type = AstJString;
  node->v.v_string = str;

  return node;
}

JNode* ast_for_jobject(Parser *ps, JArray *array) {
  JNode *node = ast_for_jarray(ps, array);
  node->type = AstJObject;

  return node;
}

JNode* ast_for_jproperty(Parser *ps, JArray *array) {
  JNode *node = ast_for_jarray(ps, array);
  node->type = AstJProperty;
  return node;
}

JNode* ast_for_jarray(Parser *ps, JArray *array) {
  JNode *node = sys_new0_N(JNode, 1);

  node->type = AstJArray;
  node->v.v_array = array;

  return node;
}

JNode* ast_for_jnode_bool(Parser *ps, bool bvalue) {
  JNode *node = sys_new0_N(JNode, 1);

  node->type = AstJBool;
  node->v.v_bool = bvalue;

  return node;
}

JNode* ast_for_jpair(Parser *ps, SysChar *key, JNode *prop, JNode *value) {
  sys_return_val_if_fail(key != NULL, NULL);
  sys_return_val_if_fail(value != NULL, NULL);

  JNode *node = sys_new0_N(JNode, 1);
  JPair *pair = sys_new0_N(JPair, 1);

  pair->key = key;
  pair->prop = prop;
  pair->value = value;

  node->type = AstJPair;
  node->v.v_pair = pair;

  return node;
}

JNode *ast_for_jdouble(Parser *ps, const SysDouble d) {
  JNode *node = sys_new0_N(JNode, 1);

  node->type = AstJDouble;
  node->v.v_double = d;

  return node;
}

JNode *ast_for_jbool(Parser *ps, bool bvalue) {
  JNode *node = sys_new0_N(JNode, 1);

  node->type = AstJBool;
  node->v.v_bool = bvalue;

  return node;
}

JNode *ast_for_jnull(Parser *ps) {
  JNode *node = sys_new0_N(JNode, 1);

  node->type = AstJNull;
  node->v.v_node = NULL;

  return node;
}

JNode *ast_for_jint(Parser *ps, const SysInt i) {
  JNode *node = sys_new0_N(JNode, 1);

  node->type = AstJInt;
  node->v.v_int = i;

  return node;
}

JNode *ast_for_jnode(Parser *ps, JNode *cnode) {
  JNode *node = sys_new0_N(JNode, 1);

  node->type = AstJNode;
  node->v.v_node = cnode;

  return node;
}

void ast_jnode_free(JNode *node) {
  sys_return_if_fail(node != NULL);
  sys_return_if_fail(node->type > 0 && node->type <= AstJInvalid);

  switch (node->type) {
    case AstJInt:
    case AstJDouble:
    case AstJNull:
    case AstJBool:
      break;
    case AstJArray:
    case AstJProperty:
    case AstJObject:
      if (node->v.v_object != NULL) {
        sys_ptr_array_free(node->v.v_object, true);
      }
      break;
    case AstJSource:
      sys_free_N(node->v.v_source->string);
      sys_free_N(node->v.v_source);
      break;
    case AstJString:
      sys_free_N(node->v.v_string);
      break;
    case AstJNode:
      ast_jnode_free(node->v.v_node);
      break;
    case AstJPair:
      ast_jpair_free(node->v.v_pair);
      break;
    case AstJInvalid:
      sys_warning_N("%s", "jnode free invalid type");
      break;
    default:
      break;
  }

  sys_free_N(node);
}

void ast_jpair_free(JPair *pair) {
  sys_return_if_fail(pair != NULL);
  sys_return_if_fail(pair->key != NULL);

  sys_free_N(pair->key);

if (pair->prop) {
  ast_jnode_free(pair->prop);
}

ast_jnode_free(pair->value);
sys_free_N(pair);
}
