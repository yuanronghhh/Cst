#include <CstCore/Parser/Ast.h>

#include <CstCore/Front/CstFrontCore.h>
#include <CstCore/Front/Common/CstComNodeBuilder.h>
#include <CstCore/Driver/Css/CstCssValue.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/Css/CstCssPair.h>
#include <CstCore/Driver/Css/CstCssEnv.h>
#include <CstCore/Driver/Css/CstCssParser.h>
#include <CstCore/Driver/Css/CstCssNode.h>

#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstComponentBuilder.h>
#include <CstCore/Driver/CstNodeBuilder.h>
#include <CstCore/Driver/CstModule.h>


typedef struct _AstModulePass AstModulePass;
typedef struct _AstComponentPass AstComponentPass;
typedef struct _AstGStylePass AstGStylePass;
typedef struct _AstNodePass AstNodePass;
typedef struct _AstKeyNode AstKeyNode;

struct _AstKeyNode {
  SysChar* key;
  SysPointer value;
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

struct _AstNodePass {
  CstNode *v_com_node;
  CstNodeBuilder *v_node_builder;
};

struct _AstGStylePass {
  const SysChar *path;
  FREnv *gcss_env;
};

struct _AstComponentPass {
  CstModule *v_module;
  CstComponent *v_component;
  CstNode *v_pnode;

  CstComponentBuilder *builder;
};

struct _AstModulePass {
  CstModule  *v_module;
  CstParser  *parser;
};

void ast_print_visitor(AstNode *node, SysPointer user_data) {
  ast_print_node(node, user_data);
}

/* AstNode */
void ast_iter_narray(SysPtrArray *array, AstNodeFunc func, SysPointer user_data) {
  for (SysUInt i = 0; i < array->len; i++) {
    AstNode *node = array->pdata[i];

    if (!func(node, user_data)) {
      break;
    }
  }
}

SysBool ast_print_node(AstNode *node, SysPointer user_data) {
  sys_return_val_if_fail(node != NULL, false);

  switch (node->type) {
    case AstRoot:
      sys_debug_N("%s", "[AstRoot]");
      break;
    case AstImport:
      sys_debug_N("%s", "[AstImport]");
      break;
    case AstSource:
      sys_debug_N("%s", "[AstSource]");
      break;
    case AstComponent:
      sys_debug_N("%s", "[AstComponent]");
      ast_print_jnode(node->v.component->property, user_data);
      ast_print_jnode(node->v.component->body, user_data);
      break;
    case AstJsonNode:
      sys_debug_N("%s", "[AstJsonNode]");
      break;
    case AstInvalid:
      sys_debug_N("%s", "[AstInvalid]");
      break;
    default:
      break;
  }

  return true;
}

AstNode* ast_for_root(CstParser *ps, JArray *array) {
  AstNode *node = sys_new0_N(AstNode, 1);

  node->type = AstRoot;
  node->v.root = array;

  return node;
}

AstNode* ast_for_import(CstParser *ps, SysPtrArray *array, SysChar *path) {
  sys_return_val_if_fail(path != NULL, NULL);

  AstNode *node = sys_new0_N(AstNode, 1);

  Import *import = sys_new0_N(Import, 1);
  import->ids = array;
  import->path = path;

  node->type = AstImport;
  node->v.import = import;

  return node;
}

AstNode* ast_for_gstyle(CstParser *ps, JNode* prop_node, JNode *body_node) {
  sys_return_val_if_fail(body_node != NULL, NULL);

  AstNode *node = sys_new0_N(AstNode, 1);
  GStyle *gstyle = sys_new0_N(GStyle, 1);

  node->type = AstGStyle;

  gstyle->property = prop_node;
  gstyle->body = body_node;

  node->v.gstyle = gstyle;

  return node;
}

AstNode* ast_for_component(CstParser *ps, JNode* prop_node, JNode *body_node) {
  sys_return_val_if_fail(prop_node != NULL, NULL);
  sys_return_val_if_fail(body_node != NULL, NULL);

  AstNode *node = sys_new0_N(AstNode, 1);

  node->type = AstComponent;
  Component *component = sys_new0_N(Component, 1);

  if(prop_node == NULL) {
    sys_abort_N("%s", "component id must be set before use");
  }

  component->property = prop_node;
  component->body = body_node;

  node->v.component = component;

  return node;
}

AstNode* ast_for_source(CstParser *ps, const SysChar *str, SysInt rows) {
  sys_return_val_if_fail(str != NULL, NULL);

  SysChar *s;
  SysInt srows = 0;

  AstNode *node = sys_new0_N(AstNode, 1);
  node->type = AstSource;
  node->v.source = sys_new0_N(Source, 1);
  node->v.source->string = (SysChar *)str;

  s = (SysChar *)str;
  while(*s) {
    if(*s == '\n') {
      srows += 1;
    }

    s++;
  }
  node->v.source->startline = rows - srows;

  return node;
}

void ast_source_free(Source* source) {
  sys_free_N(source->string);
  sys_free_N(source);
}

void ast_import_free(Import* imp) {
  sys_ptr_array_free(imp->ids, true);
  sys_free_N(imp->path);
  sys_free_N(imp);
}

void ast_node_free(AstNode* node) {
  sys_return_if_fail(node != NULL);
  sys_return_if_fail(node->type >= AstRoot && node->type <= AstInvalid);

  switch (node->type) {
    case AstRoot:
      sys_ptr_array_free(node->v.root, true);
      break;
    case AstImport:
      ast_import_free(node->v.import);
      break;
    case AstSource:
      ast_source_free(node->v.source);
      break;
    case AstComponent:
      ast_component_free(node->v.component);
      break;
    case AstGStyle:
      ast_gstyle_free(node->v.gstyle);
      break;
    case AstJsonNode:
      ast_jnode_free(node->v.jnode);
      break;
    default:
      break;
  }

  sys_free_N(node);
}

void ast_gstyle_free(GStyle* gstyle) {
  if (gstyle->property) {
    ast_jnode_free(gstyle->property);
  }
  ast_jnode_free(gstyle->body);
  sys_free_N(gstyle);
}

void ast_component_free(Component* component) {
  if (component->property) {
    ast_jnode_free(component->property);
  }
  ast_jnode_free(component->body);
  sys_free_N(component);
}

static SYS_INLINE SysBool ast_is_gstyle(AstNode *node, SysPointer user_data) {
  sys_return_val_if_fail(node != NULL, false);
  sys_return_val_if_fail(user_data != NULL, false);

  AstNode **nnode = user_data;

  if (node->type == AstGStyle) {
    *nnode = node;
    return false;
  }

  return true;
}

SysValue *ast_node_to_value(CstModule *v_module, const SysChar *key, JNode *nnode) {
  sys_return_val_if_fail(key != NULL, NULL);

  FREventFunc func;
  SysValue *v;

  v = sys_value_new();

  if(*key == '@') {
    func = cst_module_get_event_function(v_module, nnode->v.v_string);
    if (func == NULL) {
      sys_warning_N("Not found function: \"%s\" in \"%s\" module", nnode->v.v_string, cst_module_get_path(v_module));
      sys_value_free(v);
      return NULL;
    }

    sys_value_set_v_pointer(v, func);
    return v;
  }

  switch (nnode->type) {
    case AstJString:
      sys_value_set_v_string(v, nnode->v.v_string);
      break;
    case AstJInt:
      sys_value_set_v_int(v, nnode->v.v_int);
      break;
    case AstJDouble:
      sys_value_set_v_double(v, nnode->v.v_double);
      break;
    default:
      sys_warning_N("not support value type: %s", nnode->type);
      break;
  }

  return v;
}

/* CstComNode */
static SysBool com_node_parse_prop_func(JNode *jnode, CstNodeBuilder *builder) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(builder != NULL, false);

  JNode *nnode;
  CstNodeMap *map;
  CstValueMap *prop_map;
  SysValue *svalue;
  SysInt ptype;

  CstComNodeBuilder *com_builder = CST_COM_NODE_BUILDER(builder);
  sys_return_val_if_fail(com_builder != NULL, false);

  CstComponent *pcomponent = cst_node_builder_get_v_component(builder);
  sys_return_val_if_fail(pcomponent != NULL, false);

  CstModule *v_module = cst_node_builder_get_v_module(builder);
  sys_return_val_if_fail(v_module != NULL, false);

  CstComponent *component = cst_com_node_builder_get_component(com_builder);
  sys_return_val_if_fail(component != NULL, false);

  JPair *pair = jnode->v.v_pair;
  nnode = ast_jnode_jnode(pair->value);
  if (nnode == NULL) {
    return false;
  }

  ptype = cst_node_prop_get_by_name(pair->key);
  if(ptype == -1) {

    sys_warning_N("failed to parse node property: %s", pair->key);
    return false;
  }

  svalue = ast_node_to_value(v_module, pair->key, nnode);
  prop_map = cst_value_map_new_I(pair->key, sys_value_get_data_type(svalue));

  map = cst_node_map_new_I(prop_map, CST_NODE_PROP_ACTION, pair->key, svalue);
  sys_object_unref(prop_map);

  cst_com_node_builder_set_node_map(com_builder, map);

  return true;
}

void ast_com_node_parse(CstNodeBuilder *builder, JNode *jnode) {
  sys_return_if_fail(jnode->type == AstJPair);

  JPair *pair = jnode->v.v_pair;
  sys_return_if_fail(builder != NULL);

  if (pair->prop != NULL) {

    ast_iter_jobject(pair->prop, (AstJNodeFunc)com_node_parse_prop_func, builder);
  }
}

/* Component */
JNode* ast_component_body_value(JNode *node) {
  sys_return_val_if_fail(node != NULL, NULL);

  return ast_jnode_jnode(ast_jpair_value(node));
}

static SysBool component_style_node_func(JNode *jnode, CstComponentBuilder *builder) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(jnode->type == AstJPair, false);
  JPair *pair = jnode->v.v_pair;

  FREnv *env = cst_component_builder_get_css_env(builder);
  sys_return_val_if_fail(env != NULL, false);

  const SysChar *id = cst_component_builder_get_id(builder);
  sys_return_val_if_fail(id != NULL, false);

  CstCssGroup *g = ast_css_group_new_with_jpair(env, pair, true);
  if (g == NULL) {

    sys_abort_N("parse component css failed: %s, %s", id, pair->key);
  }
  cst_component_builder_set_css(builder, g);

  return true;
}

static SysBool ast_component_parse_props_func(JNode *jnode, CstComponentBuilder *builder) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(jnode->type == AstJPair, false);
  sys_return_val_if_fail(builder != NULL, false);

  JNode *nnode;
  CstValueMap *prop_map;
  SysInt ptype;

  JPair *pair = jnode->v.v_pair;
  nnode = ast_jnode_jnode(pair->value);
  if (nnode == NULL) {
    return false;
  }

  ptype = cst_value_map_parse_type(nnode->v.v_string);
  prop_map = cst_value_map_new_I(pair->key, ptype);

  cst_component_builder_set_value_map(builder, prop_map);

  return true;
}

static SysBool ast_component_parse_layout_func(JNode *jnode, CstComponentBuilder *v_comp_builder) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(v_comp_builder != NULL, false);
  sys_return_val_if_fail(ast_jnode_is_type(jnode, AstJPair), false);

  JNode *njnode;
  CstNode *v_node;
  SysType type;
  SysInt count;
  const SysChar *cus_name;
  CstNodeBuilder *v_builder;
  SysChar* tname;
  CstComponentBuilder *nbuilder;

  JPair *pair = jnode->v.v_pair;
  sys_return_val_if_fail(pair != NULL, false);

  CstModule *v_module = cst_component_builder_get_v_module(v_comp_builder);
  sys_return_val_if_fail(v_module != NULL, false);

  CstNode *v_pnode = cst_component_builder_get_v_pnode(v_comp_builder);
  sys_return_val_if_fail(v_pnode != NULL, false);

  cus_name = pair->key;

  CstComponent *child_comp = cst_module_get_component(v_module, cus_name);
  if (child_comp != NULL) {
    v_builder = cst_com_node_builder_new_I(v_module, NULL, v_pnode);
    v_node = cst_com_node_new();

    tname = sys_strdup_printf("<%s>", cst_component_builder_get_id(v_comp_builder));
    cst_node_set_name(v_node, tname);
    sys_free_N(tname);
  } else {
    type = cst_render_node_get_meta(cus_name);
    if (type == 0) {
      sys_error_N("Not found node or component \"%s\".", cus_name);
      return false;
    }

    v_builder = cst_node_builder_new_I(v_module, NULL, v_pnode);
    v_node = cst_node_new();

    cst_node_set_name(v_node, cus_name);
    cst_node_set_rnode_type(v_node, type);
  }

  cst_node_builder_parse(v_builder, jnode);
  cst_node_builder_build(v_builder, v_node);
  sys_object_unref(v_builder);

  count = cst_module_get_count(v_module);
  cst_module_set_count(v_module, ++count);

  fr_node_append(FR_NODE(v_pnode), FR_NODE(v_node));

  njnode = ast_jnode_jnode(pair->value);
  if (ast_jnode_is_type(njnode, AstJNull)) {
    return true;

  } else if (ast_jnode_is_type(njnode, AstJObject)) {
    nbuilder = cst_component_builder_new(v_module);

    ast_iter_jobject(njnode, (AstJNodeFunc)ast_component_parse_layout_func, nbuilder);

  } else {

    sys_warning_N("Not correct type in component \"%s\" pair value: %s", cus_name, pair->key);
  }

  return true;
}

static SysBool component_body_func(JNode *pair, AstComponentPass *pass) {
  sys_return_val_if_fail(pair != NULL, false);
  sys_return_val_if_fail(builder != NULL, false);

  JNode *body_node;

  CST_COMPONENT_BODY_ENUM bd = cst_component_body_get_by_name(pair->v.v_pair->key);

  switch (bd) {
    case CST_COMPONENT_BDATA:
      break;
    case CST_COMPONENT_BSTYLE:
      body_node = ast_component_body_value(pair);

      ast_iter_jobject(body_node, (AstJNodeFunc)component_style_node_func, builder);
      break;
    case CST_COMPONENT_BLAYOUT:
      body_node = ast_component_body_value(pair);

      ast_iter_jobject(body_node, (AstJNodeFunc)ast_component_parse_layout_func, builder);
      break;

    case CST_COMPONENT_BCOMPONENT:
    case CST_COMPONENT_BPROPS:
      body_node = ast_component_body_value(pair);

      ast_iter_jobject(body_node, (AstJNodeFunc)ast_component_parse_props_func, builder);
      break;
    case CST_COMPONENT_BLAST:
      break;
    default:
      break;
  }

  return true;
}

static SysBool component_property_parse_func(JNode *jnode, CstComponentBuilder *builder) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(builder != NULL, false);

  JNode *nnode;
  CST_COMPONENT_PROP_ENUM prop;

  JPair *pair = jnode->v.v_pair;

  nnode = ast_jnode_jnode(pair->value);
  if (nnode == NULL) {
    return false;
  }

  prop = cst_component_prop_get_by_name(pair->key);

  switch (prop) {
    case CST_COMPONENT_PROP_ID:
      if (nnode->type != AstJString) {
        return false;
      }

      cst_component_builder_set_id(builder, nnode->v.v_string);
      break;
    case CST_COMPONENT_PROP_BASE:
      if (nnode->type != AstJString) {
        return false;
      }

      cst_component_builder_set_base_name(builder, nnode->v.v_string);
      break;
    default:
      sys_warning_N("not support component property: %s", pair->key);
      return false;
  }

  return true;
}

SysBool ast_component_property_parse(JNode *jnode, CstComponentBuilder *builder) {
  sys_return_val_if_fail(jnode != NULL, false);

  ast_iter_jobject(jnode, (AstJNodeFunc)component_property_parse_func, builder);

  return true;
}

void ast_component_body_parse(Component *ast, AstComponentPass *pass) {

  ast_iter_jobject(ast->body, (AstJNodeFunc)component_body_func, pass);
}

/* Import */
void ast_import_parse(CstParser *ps, AstNode *node) {
  sys_return_if_fail(node->type == AstImport);

  Import *import = node->v.import;

  SysChar *cwd = sys_path_dirname(cst_parser_get_filename(ps));
  SysChar *fullpath = sys_path_join(cwd, import->path);
  sys_free_N(cwd);

  if (fullpath == NULL || !sys_path_exists(fullpath)) {
    sys_error_N("import path not found: %s", fullpath);
    abort();
  }

  SysPointer data = cst_parser_get_data(ps);
  if (data == NULL) {
    abort();
  }

  if (!cst_module_for_import(data, import->ids, fullpath)) {
    abort();
  }

  sys_free_N(fullpath);
}

/* GStyle Ast */
GStyle *ast_root_get_gstyle(AstNode *root) {
  sys_return_val_if_fail(root != NULL, NULL);
  sys_return_val_if_fail(root->type == AstRoot, NULL);

  AstNode *nnode = NULL;
  ast_iter_narray(root->v.root, ast_is_gstyle, &nnode);
  if (nnode == NULL) {
    return NULL;
  }

  return nnode->v.gstyle;
}

static SysBool ast_css_env_gstyle_func(JNode *node, AstGStylePass *pass) {
  sys_return_val_if_fail(node != NULL, false);
  sys_return_val_if_fail(pass != NULL, false);
  sys_return_val_if_fail(node->type == AstJPair, false);

  const SysChar *id;
  FREnv *gcss_env = pass->gcss_env;
  JPair *jpair = node->v.v_pair;

  CstCssGroup *g = ast_css_group_new_with_jpair(gcss_env, jpair, true);
  if (g == NULL) {
    sys_abort_N("parse css failed: %s", pass->path);
  }

  id = cst_css_group_get_id(g);
  fr_env_set(gcss_env, id, g);

  return true;
}

void ast_gstyle_parse(GStyle *gstyle, const SysChar *path) {

  AstGStylePass pass = {0};
  pass.gcss_env = cst_css_env_get_gcss_env();
  pass.path = path;

  ast_iter_jobject(gstyle->body, (AstJNodeFunc)ast_css_env_gstyle_func, (SysPointer)&pass);
}

/* Node Ast */
static SysBool node_parse_value_bind(CstNodeBuilder *builder, const SysChar *expr_str) {
  sys_return_val_if_fail(expr_str != NULL, false);

  CstValueMap *pmap = NULL;
  CstNodeMap *map;
  SysChar *index_name;
  SysInt len = (SysInt)sys_strlen(expr_str, 100);

  CstComponent *v_component = cst_node_builder_get_v_component(builder);
  sys_return_val_if_fail(v_component != NULL, false);

  // TODO: parse expr
  index_name = cst_node_builder_extract_index(expr_str, len);
  if (index_name == NULL) {
    return false;
  }

  pmap = cst_component_get_value_map(v_component, index_name);
  sys_free_N(index_name);

  if (pmap == NULL) {
    return false;
  }

  map = cst_node_map_new_I(pmap, CST_NODE_PROP_VALUE, "value", NULL);
  cst_node_builder_add_nodemap(builder, map);

  return true;
}

static SysBool node_parse_prop_func(JNode *jnode, CstNodeBuilder *builder) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(builder != NULL, false);

  JNode *nnode;
  JNode *tnode;
  SysChar **v_base;
  CST_NODE_PROP_ENUM prop;
  JPair *pair = jnode->v.v_pair;

  nnode = ast_jnode_jnode(pair->value);
  if (nnode == NULL) {
    return false;
  }

  prop = cst_node_prop_get_by_name(pair->key);

  switch (prop) {
    case CST_NODE_PROP_ID:
      if (nnode->type != AstJString) {
        return false;
      }

      cst_node_builder_set_id(builder, nnode->v.v_string);
      break;
    case CST_NODE_PROP_BASE:
      if (nnode->type != AstJArray) {
        return false;
      }

      if (nnode->v.v_array->len == 0) {
        return false;
      }

      v_base = sys_new0_N(SysChar *, nnode->v.v_array->len);

      for (SysUInt i = 0; i < nnode->v.v_array->len; i++) {
         tnode = nnode->v.v_array->pdata[i];
         if (tnode == NULL || tnode->type != AstJNode) {
           continue;
         }

         tnode = tnode->v.v_node;

         if (tnode->type != AstJString) {
           sys_warning_N("Node base property must be a string: %s", pair->key);
           sys_free_N(v_base);
           break;
         }

         v_base[i] = tnode->v.v_string;
      }

      if(!cst_node_builder_parse_base(builder, v_base, nnode->v.v_array->len)) {
        return false;
      }
      break;
    case CST_NODE_PROP_VALUE:
      if (nnode->type != AstJString) {
        return false;
      }

      if(!cst_node_builder_parse_value_bind(builder, pair->key, (const SysChar *)nnode->v.v_string)) {
        return false;
      }
      break;
    case CST_NODE_PROP_POSITION:
      if (nnode->type != AstJString) {
        return false;
      }

      if(!cst_node_builder_parse_layer_name(builder, nnode->v.v_string)) {
        return false;
      }
      break;
    case CST_NODE_PROP_ACTION:
      if (nnode->type != AstJString) {
        return false;
      }

      if(!cst_node_builder_parse_action(builder, pair->key + 1, nnode->v.v_string)) {
        return false;
      }
      break;
    case CST_NODE_PROP_LABEL:
      if (nnode->type != AstJString) {
        return false;
      }

      cst_node_builder_set_label(builder, nnode->v.v_string);
      break;
    default:
      sys_warning_N("not support node property: %s", pair->key);
      return false;
  }

  return true;
}

void ast_node_parse(CstNodeBuilder *builder, AstNode *node) {
  sys_return_if_fail(node->type == AstJsonNode);

  JNode *jnode = node->v.jnode;
  sys_return_if_fail(jnode != NULL);
  sys_return_if_fail(jnode->type == AstJPair);
  sys_return_if_fail(builder != NULL);

  if (pair->prop != NULL) {

    ast_iter_jobject(pair->prop, (AstJNodeFunc)node_parse_prop_func, builder);
  }
}

/* Css */
CstCssPair *ast_css_pair_parse(JNode *jnode) {
  sys_return_val_if_fail(jnode->type == AstJPair, NULL);

  CstCssNode* node;
  CstCssValue* value;

  JPair *jpair = jnode->v.v_pair;
  SysChar *key = jpair->key;

  sys_return_val_if_fail(key != NULL, NULL);

  node = cst_css_node_lookup(key);
  if (node == NULL) {

    sys_warning_N("Not found css by name: %s", key);
    return NULL;
  }

  value = cst_css_value_new();
  if (!ast_css_value_parse(ast_jnode_jnode(jpair->value), node, value)) {

    return NULL;
  }

  return cst_css_pair_new_I(node, value);
}

CstCssGroup* ast_css_group_new_with_jpair(FREnv *env, JPair *pair, SysBool key_lookup) {
  sys_return_val_if_fail(pair != NULL, NULL);
  sys_return_val_if_fail(pair->key != NULL, NULL);
  sys_return_val_if_fail(pair->value != NULL, NULL);
  sys_return_val_if_fail(pair->value->type == AstJNode, NULL);

  JNode *jv;
  JPair *np;
  JArray *bss;

  CstCssGroup *g, *ng;

  g = cst_css_group_new_I(pair->key);
  SysPtrArray *pv = pair->value->v.v_node->v.v_object;

  if (pair->prop != NULL) {
    jv = ast_get_jobject_value(pair->prop, "base");
    if (jv == NULL || jv->type != AstJArray) {
      goto fail;
    }
    bss = jv->v.v_array;

    for (SysUInt i = 0; i < bss->len; i++) {
      jv = ast_jnode_jnode(bss->pdata[i]);

      ng = fr_env_get_r(env, jv->v.v_string);
      if (!cst_css_group_set_base_r(g, ng)) {
        goto fail;
      }
    }
  }

  if (pair->value->type == AstJNull || pv == NULL) {
    return g;
  }

  for (SysUInt i = 0; i < pv->len; i++) {
    jv = pv->pdata[i];

    if(jv->type != AstJPair) {
      continue;
    }

    np = jv->v.v_pair;

    if (np->value->type == AstJNull || pv == NULL) {
      continue;
    }

    CstCssPair *cpair = ast_css_pair_parse(jv);
    if (cpair == NULL) {
      sys_warning_N("Failed to load css: %s", np->key);
      continue;
    }

    cst_css_group_pair_add(g, cpair);
  }

  return g;

fail:
  sys_object_unref(g);
  return NULL;
}

/* CstModule */
SysBool ast_component_parse(AstComponentPass *self, AstNode *ast) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(ast->type != AstComponent, false);
  Component *comp_ast = ast->v.component;

  CstComponent *comp;
  const SysChar *comp_id;

  AstComponentPass pass = {0};

  pass.v_component = v_component;
  pass.v_module = v_module;
  pass.v_pnode = v_pnode;

  CstComponentBuilder *builder = cst_component_builder_new_simple(v_module);
  ast_component_property_parse(ast->property, builder);

  comp_id = cst_component_builder_get_id(builder);
  if(comp_id == NULL) {
    sys_error_N("%s", "Not found id in component, maybe not init types ?");
    return false;
  }

  SysType type = cst_render_node_get_meta(comp_id);
  if (type == 0) {
    sys_error_N("Not found %s in component, maybe not init types ?", comp_id);
    return false;
  }

  comp = sys_object_new(type, NULL);
  if (comp == NULL) {
    sys_error_N("Failed to create component: %s", comp_id);
    return false;
  }

  cst_builder_parse(builder, ast);
  cst_builder_build(builder, comp);

  cst_module_set_root_component(v_module, comp);
  sys_object_unref(builder);

  return true;
}

SysBool ast_module_body_func(AstNode *body, AstModulePass *pass) {
  sys_return_val_if_fail(body != NULL, false);

  AstComponentPass comp_pass = {0};

  comp_pass->v_module = pass->v_module;

  switch(body->type) {
    case AstImport:
      break;

    case AstComponent:
      ast_component_parse(comp_pass, ast);
      break;

    case AstGStyle:
      gstyle = body->v.gstyle;

      ast_gstyle_parse(gstyle, cst_module_get_path(v_module));
      break;

    case AstSource:
      break;

    default:
      sys_error_N("Not support module root type: %s", body->type);
      break;
  }

  return true;
}

void ast_module_parse(CstParser * ps, AstNode *ast) {
  sys_return_if_fail(root != NULL);
  sys_return_if_fail(self != NULL);

  AstModulePass pass = { 0 };
  pass.v_module = cst_parser_get_data(ps);

  ast_iter_narray(root->v.root, (AstNodeFunc)ast_module_body_func, &pass);
}

/* CstCssValue */
SysBool ast_css_parse_string(SysChar *s, CstCssNode *node, CstCssValue *value) {
  SysBool r = false;
  CstCssClosure *c = NULL;

  switch (cst_css_node_get_css_ptype(node)) {
    case CST_CSS_PROP_W:
      c = cst_css_value_parse_percent(s, (CstCssFunc)cst_css_pair_width_percent);
      break;
    case CST_CSS_PROP_H:
      c = cst_css_value_parse_percent(s, (CstCssFunc)cst_css_pair_height_percent);
      break;
    default:
      break;
  }

  if (c != NULL) {

    cst_css_value_set_v_closure(value, c);
    return true;
  }

  if (sys_str_startswith(s, "#")) {

    r = ast_css_value_color_parse(s, value);
  }

  return r;
}

SysBool ast_css_value_parse(JNode *jnode, CstCssNode *css_node, CstCssValue *value) {
  sys_return_val_if_fail(value != NULL, false);
  sys_return_val_if_fail(jnode != NULL, false);

  FRSInt4 *v_m4;

  switch (jnode->type) {
    case AstJBool:
      cst_css_value_set_v_bool(value, jnode->v.v_bool);
      break;
    case AstJSource:
    case AstJString:
      if (jnode->v.v_string != NULL) {
        if(!ast_css_parse_string(jnode->v.v_string, css_node, value)) {
          cst_css_value_set_v_string(value, jnode->v.v_string);
        }
      }

      break;
    case AstJNull:
      cst_css_value_set_v_null(value);
      break;
    case AstJArray:
      if (jnode->v.v_array->len != 4) {
        break;
      }

      v_m4 = sys_new0_N(FRSInt4, 1);
      SysInt tm4[4];
      for (SysUInt i = 0; i < jnode->v.v_array->len; i++) {
        JNode *node = jnode->v.v_array->pdata[i];

        node = ast_jnode_jnode(node);
        if (node->type != AstJInt) {
          sys_warning_N("%s", "css value type should be int and length should be 4 when it is an array.");
          continue;
        }

        tm4[i] = node->v.v_int;
      }

      v_m4->m0 = tm4[0];
      v_m4->m1 = tm4[1];
      v_m4->m2 = tm4[2];
      v_m4->m3 = tm4[3];

      cst_css_value_set_v_m4(value, v_m4);
      break;
    case AstJNode:
    case AstJProperty:
    case AstJObject:
    case AstJPair:
      cst_css_value_set_v_pointer(value, (SysPointer)jnode->v.v_node);
      break;
    case AstJInt:
      cst_css_value_set_v_int(value, jnode->v.v_int);
      break;
    case AstJDouble:
      cst_css_value_set_v_double(value, jnode->v.v_double);
      break;
    default:

      sys_warning_N("Failed to parse css value with key: %s", );
      break;
  }

  return true;
}

SysBool ast_css_value_color_parse(SysChar *s, CstCssValue *value) {
  sys_return_val_if_fail(s != NULL, false);

  FRColor *color = sys_new0_N(FRColor, 1);

  if (!fr_color_rgba_parse(s, color)) {
    sys_warning_N("Faild to parse %s.", s);
    return false;
  }

  cst_css_value_set_v_color(value, color);

  return true;
}

/* CodeGen */
JNode* ast_jpair_get_by_key(JNode* jnode, SysChar* key) {
  sys_return_val_if_fail(key != NULL, NULL);

  JPair* pair = jnode->v.v_pair;

  if (sys_str_equal(pair->key, key)) {
    return pair->value;
  }

  return NULL;
}

JNode* ast_jnode_jnode(JNode* node) {
  sys_return_val_if_fail(node != NULL, NULL);
  sys_return_val_if_fail(node->type == AstJNode, NULL);

  return node->v.v_node;
}

static bool jkey_equal(JNode* node, SysPointer user_data) {
  AstKeyNode* knode = user_data;
  JPair* pair = node->v.v_pair;

  if (sys_str_equal(pair->key, knode->key)) {
    knode->value = pair->value->v.v_node;
    return false;
  }

  return true;
}

SysBool ast_jnode_is_type(JNode* jnode, JNodeType type) {
  return jnode != NULL && jnode->type == type;
}

JNode* ast_get_jobject_value(JNode* onode, SysChar* key) {
  sys_return_val_if_fail((onode->type == AstJProperty) || (onode->type == AstJObject), NULL);

  AstKeyNode kn = { key, NULL };

  ast_iter_jobject(onode, jkey_equal, &kn);

  return kn.value;
}

JNode* ast_jpair_value(JNode* jnode) {
  sys_return_val_if_fail(jnode->type == AstJPair, NULL);

  return jnode->v.v_pair->value;
}

JNode* ast_jnode_index(JNode* o, const SysChar* index_str) {
  sys_return_val_if_fail(o != NULL, NULL);

  // TODO: find info
  JNode* rs = ast_jnode_jnode(o->v.v_array->pdata[1]);

  return rs;
}

JNode* ast_jnode_clone_r(JNode* jnode) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(jnode->type > 0 && jnode->type <= AstJInvalid, false);

  JNode* nnode = NULL;

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

SysBool ast_print_jnode(JNode* node, SysPointer user_data) {
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

void ast_iter_jarray(JNode* jnode, AstJNodeFunc func, SysPointer user_data) {
  sys_return_if_fail((jnode->type == AstJObject) || (jnode->type == AstJArray) || (jnode->type == AstJProperty));
  sys_return_if_fail(func != NULL);

  JArray* array = jnode->v.v_array;
  if (array == NULL) { return; }

  for (SysInt i = 0; i < (SysInt)array->len; i++) {
    JNode* prop = array->pdata[i];

    if (!func(prop, user_data)) {
      break;
    }
  }
}

JSource* ast_jsource_new(SysChar* str) {
  JSource* source = sys_new0_N(JSource, 1);

  source->string = str;

  return source;
}

JNode* ast_for_jsource(CstParser* ps, SysChar* str) {
  sys_return_val_if_fail(str != NULL, NULL);

  JNode* node = sys_new0_N(JNode, 1);
  JSource* source = ast_jsource_new(str);

  node->type = AstJSource;
  node->v.v_source = source;

  return node;
}

JNode* ast_for_jstring(CstParser* ps, SysChar* str) {
  JNode* node = sys_new0_N(JNode, 1);

  node->type = AstJString;
  node->v.v_string = str;

  return node;
}

JNode* ast_for_jobject(CstParser* ps, JArray* array) {
  JNode* node = ast_for_jarray(ps, array);
  node->type = AstJObject;

  return node;
}

JNode* ast_for_jproperty(CstParser* ps, JArray* array) {
  JNode* node = ast_for_jarray(ps, array);
  node->type = AstJProperty;
  return node;
}

JNode* ast_for_jarray(CstParser* ps, JArray* array) {
  JNode* node = sys_new0_N(JNode, 1);

  node->type = AstJArray;
  node->v.v_array = array;

  return node;
}

JNode* ast_for_jnode_bool(CstParser* ps, bool bvalue) {
  JNode* node = sys_new0_N(JNode, 1);

  node->type = AstJBool;
  node->v.v_bool = bvalue;

  return node;
}

JNode* ast_for_jpair(CstParser* ps, SysChar* key, JNode* prop, JNode* value) {
  sys_return_val_if_fail(key != NULL, NULL);
  sys_return_val_if_fail(value != NULL, NULL);

  JNode* node = sys_new0_N(JNode, 1);
  JPair* pair = sys_new0_N(JPair, 1);

  pair->key = key;
  pair->prop = prop;
  pair->value = value;

  node->type = AstJPair;
  node->v.v_pair = pair;

  return node;
}

JNode* ast_for_jdouble(CstParser* ps, const SysDouble d) {
  JNode* node = sys_new0_N(JNode, 1);

  node->type = AstJDouble;
  node->v.v_double = d;

  return node;
}

JNode* ast_for_jbool(CstParser* ps, bool bvalue) {
  JNode* node = sys_new0_N(JNode, 1);

  node->type = AstJBool;
  node->v.v_bool = bvalue;

  return node;
}

JNode* ast_for_jnull(CstParser* ps) {
  JNode* node = sys_new0_N(JNode, 1);

  node->type = AstJNull;
  node->v.v_node = NULL;

  return node;
}

JNode* ast_for_jint(CstParser* ps, const SysInt i) {
  JNode* node = sys_new0_N(JNode, 1);

  node->type = AstJInt;
  node->v.v_int = i;

  return node;
}

JNode* ast_for_jnode(CstParser* ps, JNode* cnode) {
  JNode* node = sys_new0_N(JNode, 1);

  node->type = AstJNode;
  node->v.v_node = cnode;

  return node;
}

void ast_jnode_free(JNode* node) {
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

void ast_jpair_free(JPair* pair) {
  sys_return_if_fail(pair != NULL);
  sys_return_if_fail(pair->key != NULL);

  sys_free_N(pair->key);

  if (pair->prop) {
    ast_jnode_free(pair->prop);
  }

  ast_jnode_free(pair->value);
  sys_free_N(pair);
}
