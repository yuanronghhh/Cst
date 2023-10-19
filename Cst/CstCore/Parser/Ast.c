#include <CstCore/Parser/Ast.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstComponentBuilder.h>
#include <CstCore/Driver/CstNodeBuilder.h>
#include <CstCore/Driver/CstPropPair.h>
#include <CstCore/Front/CstFrontCore.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstManager.h>
#include <CstCore/Driver/Css/CstCssValue.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/Css/CstCssPair.h>
#include <CstCore/Driver/Css/CstCssEnv.h>
#include <CstCore/Driver/Css/CstCssParser.h>
#include <CstCore/Driver/CstRenderNode.h>


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
  CstComponent *v_component;
  CstNode *v_com_node;
  CstNodeBuilder *v_builder;
};

struct _AstGStylePass {
  const SysChar *path;
  CstCssEnv *gcss_env;
};

struct _AstComponentPass {
  CstModule     *v_module;
  CstComponent  *v_component;
  CstNode       *v_pnode;
  CstComponentBuilder *v_comp_bd;
};

struct _AstModulePass {
  CstModule  *v_module;
  CstCssEnv* gcss_env;
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

/* CstComNode */
static SysBool com_node_parse_prop_func(JNode *jnode, AstNodePass *pass) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(pass != NULL, false);

  JNode *nnode;
  CstComNode *v_com_node;
  FREventFunc watch_func;
  SysChar *func_name;
  CstPropValue *svalue;

  CstNode * v_node = pass->v_com_node;
  sys_return_val_if_fail(v_node != NULL, false);

  v_com_node = CST_COM_NODE(v_node);

  CstComponent *pcomponent = pass->v_component;
  sys_return_val_if_fail(pcomponent != NULL, false);

  CstComponent *component = cst_com_node_get_component(v_com_node);
  sys_return_val_if_fail(component != NULL, false);

  JPair *pair = jnode->v.v_pair;
  nnode = ast_jnode_jnode(pair->value);
  if (nnode == NULL) {
    return false;
  }

  if(*pair->key == '@') {
    if(nnode->type != AstJString) {
      return false;
    }

    func_name = sys_strdup_printf("%s%s", FR_FUNC_EVENT_PREFIX, (nnode->v.v_string));
    watch_func = (FREventFunc)cst_component_get_function(pcomponent, func_name);
    sys_free_N(func_name);

    if (watch_func == NULL) {
      sys_warning_N("Not found function: \"%s\" in \"%s\" component",
          (nnode->v.v_string), cst_component_get_id(pcomponent));

      return false;
    }

    svalue = sys_value_new_pointer((SysPointer)watch_func);

  } else {

    svalue = ast_jnode_new_value(nnode);
  }

  com_node_set_value(v_com_node, pair->key, svalue);

  return true;
}

void ast_com_node_parse(JNode *jnode, CstComponent *component, CstNode *v_com_node, CstNodeBuilder *builder) {
  sys_return_if_fail(jnode->type == AstJPair);

  JPair *pair = jnode->v.v_pair;
  sys_return_if_fail(component != NULL);
  sys_return_if_fail(builder != NULL);

  AstNodePass pass = { 0 };
  pass.v_component = component;
  pass.v_builder = builder;
  pass.v_com_node = v_com_node;

  if (pair->prop != NULL) {

    ast_iter_jobject(pair->prop, (AstJNodeFunc)com_node_parse_prop_func, &pass);
  }
}

/* Component */
JNode* ast_component_body_value(JNode *node) {
  sys_return_val_if_fail(node != NULL, NULL);

  return ast_jnode_jnode(ast_jpair_value(node));
}

static SysBool component_style_node_func(JNode *jnode, CstComponent *self) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(jnode->type == AstJPair, false);
  JPair *pair = jnode->v.v_pair;
  CstCssEnv *env = (CstCssEnv *)cst_component_get_css_env(self);

  const SysChar *id = cst_component_get_id(self);

  CstCssGroup *g = ast_css_group_new_with_jpair(env, pair, true);
  if (g == NULL) {
    sys_abort_N("parse component css failed: %s, %s", id, pair->key);
  }
  cst_component_set_css(self, g);

  return true;
}

CstPropValue* ast_jnode_new_value(JNode *jnode) {
  CstPropValue* o = NULL;

  switch (jnode->type) {
    case AstJString:
      o = sys_value_new_string(jnode->v.v_string);
      break;
    default:
      break;
  }

  return o;
}

static SysBool ast_component_parse_props_func(JNode *jnode, AstComponentPass *pass) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(jnode->type == AstJPair, false);
  sys_return_val_if_fail(pass != NULL, false);

  JNode *nnode;
  CstPropMap *prop_map;
  SysInt ptype;

  CstComponent *component = pass->v_component;
  sys_return_val_if_fail(component != NULL, false);

  JPair *pair = jnode->v.v_pair;
  nnode = ast_jnode_jnode(pair->value);
  if (nnode == NULL) {
    return false;
  }


  ptype = cst_prop_map_parse_type(pair->key, nnode->v.v_string);
  prop_map = cst_prop_map_new_I(pair->key, ptype);

  cst_component_set_props_map(component, prop_map);

  return true;
}

static SysBool ast_component_parse_layout_func(JNode *jnode, AstComponentPass *pass) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(pass != NULL, false);
  sys_return_val_if_fail(ast_jnode_is_type(jnode, AstJPair), false);

  SysObject *o;
  JNode *njnode;
  CstNode *v_node;
  CstNode *v_parent;
  SysType type;
  const SysChar *cus_name;
  SysChar *tname;
  CstNodeBuilder *builder;

  JPair *pair = jnode->v.v_pair;
  sys_return_val_if_fail(pair != NULL, false);

  CstModule *v_module = pass->v_module;
  sys_return_val_if_fail(v_module != NULL, false);

  CstComponent *v_component = pass->v_component;
  sys_return_val_if_fail(v_component != NULL, false);

  cus_name = pair->key;
  v_parent = pass->v_pnode;

  builder = cst_node_builder_new_I(v_module, v_component, v_parent);

  CstComponent *child_comp = cst_module_get_comp(v_module, cus_name);
  if (child_comp != NULL) {
    cus_name = cst_component_get_id(child_comp);

    v_node = cst_com_node_new_with_component(child_comp);
    ast_com_node_parse(jnode, v_component, v_node, builder);

    tname = sys_strdup_printf("<%s>", pair->key);
    cst_node_set_name(v_node, tname);
    sys_free_N(tname);
  } else {

    type = cst_node_get_meta(cus_name);
    if (type == 0) {
      sys_error_N("Not found node or component \"%s\".", cus_name);
      return false;
    }

    o = sys_object_new(type, NULL);
    if (o == NULL) {
      return false;
    }

    v_node = CST_NODE(o);

    ast_node_parse(jnode, v_component, builder);
  }

  cst_node_construct(v_node, builder);

  cst_module_count_inc(v_module);
  cst_node_append(v_parent, v_node);

  njnode = ast_jnode_jnode(pair->value);
  if (ast_jnode_is_type(njnode, AstJNull)) {
    return true;

  } else if (ast_jnode_is_type(njnode, AstJObject)) {

    AstComponentPass npass = *pass;
    npass.v_pnode = v_node;

    ast_iter_jobject(njnode, (AstJNodeFunc)ast_component_parse_layout_func, &npass);

  } else {

    sys_warning_N("Not correct type in component \"%s\" pair value: %s", cus_name, pair->key);
  }

  return true;
}

static SysBool component_body_func(JNode *pair, AstComponentPass *pass) {
  sys_return_val_if_fail(pair != NULL, false);
  sys_return_val_if_fail(pass != NULL, false);

  CstComponent *self = pass->v_component;
  sys_return_val_if_fail(self != NULL, false);

  JNode *body_node;

  CST_COMPONENT_BODY_ENUM bd = cst_component_body_get_by_name(pair->v.v_pair->key);

  switch (bd) {
    case CST_COMPONENT_BDATA:
      break;
    case CST_COMPONENT_BSTYLE:
      body_node = ast_component_body_value(pair);

      ast_iter_jobject(body_node, (AstJNodeFunc)component_style_node_func, self);
      break;
    case CST_COMPONENT_BLAYOUT:
      body_node = ast_component_body_value(pair);

      ast_iter_jobject(body_node, (AstJNodeFunc)ast_component_parse_layout_func, pass);

      cst_component_set_layout_node(self, pass->v_pnode);
      break;

    case CST_COMPONENT_BCOMPONENT:
    case CST_COMPONENT_BPROPS:
      body_node = ast_component_body_value(pair);

      ast_iter_jobject(body_node, (AstJNodeFunc)ast_component_parse_props_func, pass);
      break;
    case CST_COMPONENT_BLAST:
      break;
    default:
      break;
  }

  return true;
}

static SysBool component_property_parse_func(JNode *jnode, AstComponentPass *pass) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(pass != NULL, false);

  JNode *nnode;
  CST_COMPONENT_PROP_ENUM prop;

  CstComponentBuilder *bd = pass->v_comp_bd;
  sys_return_val_if_fail(bd != NULL, false);

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

      cst_component_builder_set_id(bd, nnode->v.v_string);
      break;
    case CST_COMPONENT_PROP_BASE:
      if (nnode->type != AstJString) {
        return false;
      }

      cst_component_builder_set_base_name(bd, nnode->v.v_string);
      break;
    default:
      sys_warning_N("not support component property: %s", pair->key);
      return false;
  }

  return true;
}

SysBool ast_component_property_parse(JNode *jnode, CstComponentBuilder *v_comp_bd) {
  sys_return_val_if_fail(jnode != NULL, false);

  AstComponentPass pass = { 0 };
  pass.v_comp_bd = v_comp_bd;

  ast_iter_jobject(jnode, (AstJNodeFunc)component_property_parse_func, &pass);

  return true;
}

static void ast_component_body_parse(Component *ast, CstComponent *o, CstModule *v_module) {
  AstComponentPass pass = {0};
  CstComponentBuilder *v_comp_bd;
  CstNodeBuilder *v_node_bd;

  CstNode *layout = cst_node_new();

  v_node_bd = cst_node_builder_new_I(v_module, o, NULL);

  cst_node_set_name(layout, "<layout-node>");
  cst_node_construct(layout, v_node_bd);

  v_comp_bd = cst_component_builder_new();
  pass.v_pnode = layout;
  pass.v_module = v_module;
  pass.v_component = o;
  pass.v_comp_bd = v_comp_bd;

  ast_iter_jobject(ast->body, (AstJNodeFunc)component_body_func, (SysPointer)&pass);
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
GStyle *ast_root_get_gstyle(AstNode *root)
 {
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
  CstCssEnv *gcss_env = pass->gcss_env;
  JPair *jpair = node->v.v_pair;

  CstCssGroup *g = ast_css_group_new_with_jpair(gcss_env, jpair, true);
  if (g == NULL) {
    sys_abort_N("parse css failed: %s", pass->path);
  }

  id = cst_css_group_get_id(g);
  cst_css_env_set(gcss_env, id, g);

  return true;
}

void ast_gstyle_parse(GStyle *gstyle, CstCssEnv *gcss_env, const SysChar *path) {

  AstGStylePass pass = {0};
  pass.gcss_env = gcss_env;
  pass.path = path;

  ast_iter_jobject(gstyle->body, (AstJNodeFunc)ast_css_env_gstyle_func, (SysPointer)&pass);
}

/* Node Ast */
static void node_parse_action_bind(CstNodeBuilder *builder, const SysChar *watch_name, const SysChar *func_name, CstComponent *component, SysChar **bind_var) {
  sys_return_if_fail(func_name != NULL);

  CstPropMap *pmap = NULL;
  CstNodeMap *map;
  SysChar *index_name;
  SysInt len = (SysInt)sys_strlen(func_name, 100);

  index_name = ast_jnode_extract_index(func_name, len);
  if (index_name == NULL) {
    return;
  }
  *bind_var = index_name;

  pmap = cst_component_get_props_map(component, index_name);
  if (pmap == NULL) {
    sys_error_N("Not found props in component: %s, %s", cst_component_get_id(component), index_name);
    *bind_var = NULL;
    sys_free_N(index_name);
    return;
  }

  map = cst_node_map_new_I(pmap, CST_NODE_PROP_ACTION, watch_name);
  cst_node_builder_add_node_maps(builder, map);
}

static void node_parse_action(CstNodeBuilder *builder, const SysChar *watch_name, const SysChar *func_name, CstComponent *component) {
  sys_return_if_fail(func_name != NULL);
  sys_return_if_fail(watch_name != NULL);

  SysChar *fname;
  FREventFunc watch_func = NULL;
  FRAWatch *awatch;
  SysChar *bind_var = NULL;

  if(*func_name == '{') {
    node_parse_action_bind(builder, watch_name, func_name, component, &bind_var);

  } else {

    fname = sys_strdup_printf("%s%s", FR_FUNC_EVENT_PREFIX, func_name);
    watch_func = (FREventFunc)cst_component_get_function(component, fname);
    sys_free_N(fname);

    if (watch_func == NULL) {
      sys_warning_N("Not found function: \"%s\" in \"%s\" component",
        func_name, cst_component_get_id(component));
      return;
    }

    bind_var = sys_strdup(func_name);
  }

  FRAWatchProps awatch_props = {0};
  awatch_props.get_bound_func = (FRGetBoundFunc)cst_layout_node_get_bound;

  awatch = fr_awatch_new_by_name(watch_name, bind_var, watch_func, &awatch_props);
  sys_clear_pointer(&bind_var, sys_free);

  if (awatch == NULL) {
    sys_warning_N("Not found action: \"%s\" in \"%s\" component",
      watch_name, cst_component_get_id(component));
  }

  cst_node_builder_add_awatches(builder, awatch);
}

static SysBool node_parse_value_bind(CstNodeBuilder *builder, CstComponent *component, const SysChar *expr_str) {
  sys_return_val_if_fail(expr_str != NULL, false);

  CstPropMap *pmap = NULL;
  CstNodeMap *map;
  SysChar *index_name;
  SysInt len = (SysInt)sys_strlen(expr_str, 100);

  // TODO: parse expr
  index_name = ast_jnode_extract_index(expr_str, len);
  if (index_name == NULL) {
    return false;
  }

  pmap = cst_component_get_props_map(component, index_name);
  sys_free_N(index_name);

  if (pmap == NULL) {
    return false;
  }

  map = cst_node_map_new_I(pmap, CST_NODE_PROP_VALUE, "value");
  cst_node_builder_add_node_maps(builder, map);

  return true;
}

static SysBool node_parse_prop_func(JNode *jnode, AstNodePass *pass) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(pass != NULL, false);

  JNode *nnode;
  JNode *tnode;
  SysChar **v_base;
  SysInt position;
  CST_NODE_PROP_ENUM prop;

  CstComponent *component = pass->v_component;
  sys_return_val_if_fail(component != NULL, false);

  CstNodeBuilder *builder = pass->v_builder;
  sys_return_val_if_fail(builder != NULL, false);

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

      cst_node_builder_set_base(builder, v_base, nnode->v.v_array->len);
      break;
    case CST_NODE_PROP_VALUE:
      if (nnode->type != AstJString) {
        return false;
      }

      node_parse_value_bind(builder, component, (const SysChar *)nnode->v.v_string);
      cst_node_builder_set_value(builder, nnode->v.v_string);
      break;
    case CST_NODE_PROP_ABSOLUTE:
      if (nnode->type != AstJInt) {
        return false;
      }

      position = cst_render_node_type_by_name(nnode->v.v_string);
      cst_node_builder_set_position(builder, position);
      break;
    case CST_NODE_PROP_ACTION:
      if (nnode->type != AstJString) {
        return false;
      }

      node_parse_action(builder, pair->key + 1, nnode->v.v_string, component);

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

void ast_node_parse(JNode *jnode, CstComponent *component, CstNodeBuilder *builder) {
  sys_return_if_fail(jnode->type == AstJPair);

  JPair *pair = jnode->v.v_pair;
  sys_return_if_fail(component != NULL);
  sys_return_if_fail(builder != NULL);

  AstNodePass pass = { 0 };
  pass.v_component = component;
  pass.v_builder = builder;

  if (pair->prop != NULL) {

    ast_iter_jobject(pair->prop, (AstJNodeFunc)node_parse_prop_func, &pass);
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
  if (ast_css_value_parse(ast_jnode_jnode(jpair->value), key, value)) {
    return NULL;
  }

  return cst_css_pair_new_I(node, value);
}

CstCssGroup* ast_css_group_new_with_jpair(CstCssEnv *env, JPair *pair, SysBool key_lookup) {
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

      ng = cst_css_env_get_r(env, jv->v.v_string);
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
      sys_warning_N("Failed to load css pair: %s", np->key);
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
static SysBool ast_module_parse_component(CstModule *v_module, Component *comp_ast) {
  CstComponent *comp;
  const SysChar *comp_id;

  CstComponentBuilder *comp_bd = cst_component_builder_new();
  ast_component_property_parse(comp_ast->property, comp_bd);

  comp_id = cst_component_builder_get_id(comp_bd);
  if(comp_id == NULL) {
    sys_error_N("%s", "Not found id in component, maybe not init types ?");
    return false;
  }

  SysType type = cst_node_get_meta(comp_id);
  if (type == 0) {
    sys_error_N("Not found %s in component, maybe not init types ?", comp_id);
    return false;
  }

  comp = sys_object_new(type, NULL);
  if (comp == NULL) {
    sys_error_N("Failed to create component: %s", comp_id);
    return false;
  }

  cst_component_set_id(comp, comp_id);
  cst_component_construct(comp, v_module, NULL);
  ast_component_body_parse(comp_ast, comp, v_module);
  cst_module_set_root_comp(v_module, comp);

  return true;
}

SysBool ast_module_body_func(AstNode *body, AstModulePass *pass) {
  sys_return_val_if_fail(body != NULL, false);

  GStyle *gstyle;
  Component *comp_ast;
  CstModule *v_module = pass->v_module;

  switch(body->type) {
    case AstImport:
      break;

    case AstComponent:
      comp_ast = body->v.component;
      ast_module_parse_component(v_module, comp_ast);
      break;

    case AstGStyle:
      gstyle = body->v.gstyle;

      ast_gstyle_parse(gstyle, pass->gcss_env, cst_module_get_path(v_module));
      break;

    case AstSource:
      break;

    default:
      sys_error_N("Not support module root type: %s", body->type);
      break;
  }

  return true;
}

void ast_module_parse(AstNode *root, CstModule *self) {
  sys_return_if_fail(root != NULL);
  sys_return_if_fail(self != NULL);

  AstModulePass pass = { 0 };
  pass.v_module = self;

  ast_iter_narray(root->v.root, (AstNodeFunc)ast_module_body_func, &pass);

}

/* CstCssValue */
SysBool ast_css_parse_string(SysChar *s, SysChar *key, CstCssValue *value) {
  SysBool r = false;
  CstCssClosure *c = NULL;
  CST_CSS_PROP_ENUM prop = cst_css_node_get_css_ptype(key);

  switch (prop) {
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
    r = true;
  } else {
    if (sys_str_startswith(s, "#")) {

      r = ast_css_value_color_parse(s, value);
    }
  }


  return r;
}

SysInt ast_css_value_parse(JNode *jnode, SysChar *key, CstCssValue *value) {
  sys_return_val_if_fail(value != NULL, SYS_FAILED);
  sys_return_val_if_fail(jnode != NULL, SYS_FAILED);

  FRSInt4 *v_m4;

  switch (jnode->type) {
    case AstJBool:
      cst_css_value_set_v_bool(value, jnode->v.v_bool);
      break;
    case AstJSource:
    case AstJString:
      if (jnode->v.v_string != NULL) {
        ast_css_parse_string(jnode->v.v_string, key, value);
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

      sys_warning_N("Failed to parse css value with key: %s", key);
      break;
  }

  return SYS_SUCCESS;
}

SysBool ast_css_value_color_parse(SysChar *s, CstCssValue *value) {
  sys_return_val_if_fail(s != NULL, SYS_FAILED);

  FRColor *color = sys_new0_N(FRColor, 1);

  if (!fr_color_rgba_parse(s, color)) {
    sys_warning_N("Faild to parse %s.", s);
    return SYS_FAILED;
  }

  cst_css_value_set_v_color(value, color);

  return SYS_SUCCESS;
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

/* JNode */
SysChar* ast_jnode_extract_index(const SysChar* str, SysInt slen) {
  SysChar* sp;
  SysChar* nsp;

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
