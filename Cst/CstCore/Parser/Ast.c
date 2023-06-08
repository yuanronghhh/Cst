#include <CstCore/Parser/Ast.h>
#include <CstCore/Front/Common/CstComponent.h>
#include <CstCore/Front/Common/CstNodeMapCore.h>
#include <CstCore/Front/CstFrontCore.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstManager.h>
#include <CstCore/Driver/Css/CstCss.h>


typedef struct _AstModulePass AstModulePass;
typedef struct _AstComponentPass AstComponentPass;
typedef struct _AstGStylePass AstGStylePass;
typedef struct _AstNodePass AstNodePass;


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

struct _Expression {
  SysInt reserved;
};

struct _AstNode {
  AstNodeType type;

  union {
    SysPtrArray *root;
    Import *import;
    Source *source;
    Component *component;
    GStyle *gstyle;
    Expression *expr;
    JNode *jnode;
  } v;
};

struct _AstNodePass {
  CstComponent *v_component;
  CstNode *v_com_node;
  CstNodeProps *v_props;
};

struct _AstGStylePass {
  const SysChar *path;
  CstCssEnv *gcss_env;
};

struct _AstComponentPass {
  CstManager    *v_manager;
  CstModule     *v_module;
  CstComponent  *v_component;
  CstNode       *v_pnode;
  CstComponentProps *v_props;
};

struct _AstModulePass {
  CstModule  *v_module;
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

AstNode* ast_for_root(Parser *ps, JArray *array) {
  AstNode *node = sys_new0_N(AstNode, 1);

  node->type = AstRoot;
  node->v.root = array;

  return node;
}

AstNode* ast_for_import(Parser *ps, SysPtrArray *array, SysChar *path) {
  sys_return_val_if_fail(path != NULL, NULL);

  AstNode *node = sys_new0_N(AstNode, 1);

  Import *import = sys_new0_N(Import, 1);
  import->ids = array;
  import->path = path;

  node->type = AstImport;
  node->v.import = import;

  return node;
}

AstNode* ast_for_gstyle(Parser *ps, JNode* prop_node, JNode *body_node) {
  sys_return_val_if_fail(body_node != NULL, NULL);

  AstNode *node = sys_new0_N(AstNode, 1);
  GStyle *gstyle = sys_new0_N(GStyle, 1);

  node->type = AstGStyle;

  gstyle->property = prop_node;
  gstyle->body = body_node;

  node->v.gstyle = gstyle;

  return node;
}

AstNode* ast_for_component(Parser *ps, JNode* prop_node, JNode *body_node) {
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

AstNode* ast_for_source(Parser *ps, const SysChar *str, SysInt rows) {
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

  CstNode * v_node = pass->v_com_node;
  sys_return_val_if_fail(v_node != NULL, false);

  v_com_node = CST_COM_NODE(v_node);

  CstComponent *component = cst_com_node_get_component(v_com_node);
  sys_return_val_if_fail(component != NULL, false);

  JPair *pair = jnode->v.v_pair;
  nnode = ast_jnode_jnode(pair->value);
  if (nnode == NULL) {
    return false;
  }

  SysValue *value = ast_jnode_new_value(nnode);
  com_node_set_value(v_com_node, pair->key, value);

  return true;
}

void ast_com_node_parse(JNode *jnode, CstComponent *component, CstNode *v_com_node, CstNodeProps *props) {
  sys_return_if_fail(jnode->type == AstJPair);

  JPair *pair = jnode->v.v_pair;
  sys_return_if_fail(component != NULL);
  sys_return_if_fail(props != NULL);

  AstNodePass pass = { 0 };
  pass.v_component = component;
  pass.v_props = props;
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
  CstCssEnv *env = cst_component_get_css_env(self);

  const SysChar *id = cst_component_get_id(self);

  CstCssGroup *g = ast_css_group_new_with_jpair(env, pair, true);
  if (g == NULL) {
    sys_abort_N("parse component css failed: %s, %s", id, pair->key);
  }
  cst_component_set_css(self, g);

  return true;
}

void cst_node_props_destroy(CstNodeProps *props) {
  if (props->v_base_len > 0) {
    sys_free_N(props->v_base);
  }
}

SysValue* ast_jnode_new_value(JNode *jnode) {
  SysValue* o = NULL;

  switch (jnode->type) {
    case AstJString:
      o = sys_value_new_string(jnode->v.v_string);
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
  SysValue *value;

  CstComponent *component = pass->v_component;
  sys_return_val_if_fail(component != NULL, false);

  JPair *pair = jnode->v.v_pair;
  nnode = ast_jnode_jnode(pair->value);
  if (nnode == NULL) {
    return false;
  }

  value = ast_jnode_new_value(nnode);

  prop_map = cst_prop_map_new_I(pair->key, value);
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
  CstNodeProps v_props = { 0 };

  JPair *pair = jnode->v.v_pair;
  sys_return_val_if_fail(pair != NULL, false);

  CstModule *v_module = pass->v_module;
  sys_return_val_if_fail(v_module != NULL, false);

  CstManager *v_manager = pass->v_manager;
  sys_return_val_if_fail(v_manager != NULL, false);

  CstComponent *v_component = pass->v_component;
  sys_return_val_if_fail(v_component != NULL, false);

  cus_name = pair->key;
  v_parent = pass->v_pnode;

  CstComponent *child_comp = cst_module_get_comp(v_module, cus_name);
  if (child_comp != NULL) {
    cus_name = cst_component_get_id(child_comp);

    v_node = cst_com_node_new_with_component(child_comp);
    ast_com_node_parse(jnode, v_component, v_node, &v_props);

    tname = sys_strdup_printf("<%s>", pair->key);
    cst_node_set_name(v_node, tname);
    sys_free_N(tname);
  } else {

    cst_manager_lock(v_manager);
    type = cst_manager_get_meta(v_manager, cus_name);
    cst_manager_unlock(v_manager);
    if (type == 0) {
      sys_error_N("Not found node or component \"%s\".", cus_name);
      return false;
    }

    o = sys_object_new(type, NULL);
    if (o == NULL) {
      return false;
    }

    v_node = CST_NODE(o);
    ast_node_parse(jnode, v_component, &v_props);
  }

  cst_node_construct(v_module, v_component, v_parent, v_node, &v_props);
  cst_node_props_destroy(&v_props);

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

  CstComponentProps *props = pass->v_props;
  sys_return_val_if_fail(props != NULL, false);

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

      props->v_id = nnode->v.v_string;
      break;
    case CST_COMPONENT_PROP_BASE:
      if (nnode->type != AstJString) {
        return false;
      }

      props->v_base_name = nnode->v.v_string;
      break;
    default:
      sys_warning_N("not support component property: %s", pair->key);
      return false;
  }

  return true;
}

SysBool ast_component_property_parse(JNode *jnode, CstComponentProps *v_props) {
  sys_return_val_if_fail(jnode != NULL, false);

  AstComponentPass pass = { 0 };
  pass.v_props = v_props;

  ast_iter_jobject(jnode, (AstJNodeFunc)component_property_parse_func, &pass);

  return true;
}

static void ast_component_body_parse(Component *ast, CstComponent *o, CstModule *v_module) {
  AstComponentPass pass = {0};
  CstNodeProps props = { 0 };
  CstNode *layout = cst_node_new();

  pass.v_manager = cst_module_get_manager(v_module);
  pass.v_module = v_module;
  pass.v_component = o;

  props.v_id = cst_component_get_id(o);

  cst_node_set_name(layout, "<layout-node>");
  cst_node_construct(v_module, o, NULL, layout, &props);

  pass.v_pnode = layout;

  ast_iter_jobject(ast->body, (AstJNodeFunc)component_body_func, (SysPointer)&pass);
}

/* Import */
void ast_import_parse(Parser *ps, AstNode *node) {
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
static void node_parse_action_bind(CstNodeProps *props, const SysChar *watch_name, const SysChar *func_name, CstComponent *component) {
  sys_return_if_fail(func_name != NULL);

  CstPropMap *pmap = NULL;
  CstNodeMap *map;
  SysChar *index_name;
  SysInt len = (SysInt)sys_strlen(func_name, 100);

  index_name = ast_jnode_extract_index(func_name, len);
  if (index_name == NULL) {
    return;
  }

  pmap = cst_component_get_props_map(component, index_name);
  sys_free_N(index_name);

  if (pmap == NULL) {
    return;
  }

  map = cst_node_map_new_I(pmap, CST_NODE_PROP_ACTION);
  props->v_node_maps = sys_list_prepend(props->v_node_maps, map);
}

static void node_parse_action(CstNodeProps *props, const SysChar *watch_name, const SysChar *func_name, CstComponent *component) {
  sys_return_if_fail(func_name != NULL);
  sys_return_if_fail(watch_name != NULL);

  SysChar *fname;
  FREventFunc watch_func;
  FRAWatch *awatch;

  if(*func_name == '{') {
    node_parse_action_bind(props, watch_name, func_name, component);
    return;
  }

  fname = sys_strdup_printf("%s%s", FR_FUNC_EVENT_PREFIX, func_name);
  watch_func = cst_component_get_function(component, fname);
  sys_free_N(fname);

  if (watch_func == NULL) {
    sys_warning_N("Not found function: \"%s\" in \"%s\" component",
      func_name, cst_component_get_id(component));
    return;
  }

  FRAWatchProps awatch_props = {0};
  awatch_props.get_bound_func = (FRGetBoundFunc)cst_node_get_bound_bp;

  awatch = fr_awatch_new_by_name(watch_name, func_name, watch_func, &awatch_props);
  if (awatch == NULL) {
    sys_warning_N("Not found action: \"%s\" in \"%s\" component",
      watch_name, cst_component_get_id(component));
  }

  props->v_awatches = sys_list_prepend(props->v_awatches, awatch);
}

static void node_parse_value_bind(CstNodeProps *props, CstComponent *component, const SysChar *expr_str) {
  sys_return_if_fail(expr_str != NULL);

  CstPropMap *pmap = NULL;
  CstNodeMap *map;
  SysChar *index_name;
  SysInt len = (SysInt)sys_strlen(expr_str, 100);

  // TODO: parse expr
  index_name = ast_jnode_extract_index(expr_str, len);
  if (index_name == NULL) {
    return;
  }

  pmap = cst_component_get_props_map(component, index_name);
  sys_free_N(index_name);

  if (pmap == NULL) {
    return;
  }

  map = cst_node_map_new_I(pmap, CST_NODE_PROP_VALUE);
  props->v_node_maps = sys_list_prepend(props->v_node_maps, map);
}

static SysBool node_parse_prop_func(JNode *jnode, AstNodePass *pass) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(pass != NULL, false);

  JNode *nnode;
  JNode *tnode;
  CST_NODE_PROP_ENUM prop;

  CstComponent *component = pass->v_component;
  sys_return_val_if_fail(component != NULL, false);

  CstNodeProps *props = pass->v_props;
  sys_return_val_if_fail(props != NULL, false);

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

      props->v_id = nnode->v.v_string;
      break;
    case CST_NODE_PROP_BASE:
      if (nnode->type != AstJArray) {
        return false;
      }

      if (nnode->v.v_array->len == 0) {
        return false;
      }

      props->v_base = sys_new0_N(SysChar *, nnode->v.v_array->len);

      for (SysUInt i = 0; i < nnode->v.v_array->len; i++) {
         tnode = nnode->v.v_array->pdata[i];
         if (tnode == NULL || tnode->type != AstJNode) {
           continue;
         }

         tnode = tnode->v.v_node;

         if (tnode->type != AstJString) {
           sys_warning_N("Node base property must be a string: %s", pair->key);
           sys_free_N(props->v_base);
           break;
         }

         props->v_base[i] = tnode->v.v_string;
      }

      props->v_base_len = nnode->v.v_array->len;
      break;
    case CST_NODE_PROP_VALUE:
      if (nnode->type != AstJString) {
        return false;
      }

      props->v_value = nnode->v.v_string;
      node_parse_value_bind(props, component, (const SysChar *)props->v_value);

      break;
    case CST_NODE_PROP_ABSOLUTE:
      if (nnode->type != AstJInt) {
        return false;
      }

      props->v_absolute = !!(nnode->v.v_int);
      break;
    case CST_NODE_PROP_ACTION:
      if (nnode->type != AstJString) {
        return false;
      }

      node_parse_action(props, pair->key + 1, nnode->v.v_string, component);

      break;
    case CST_NODE_PROP_LABEL:
      if (nnode->type != AstJString) {
        return false;
      }

      props->v_label = nnode->v.v_string;
      break;
    default:
      sys_warning_N("not support node property: %s", pair->key);
      return false;
  }

  return true;
}

void ast_node_parse(JNode *jnode, CstComponent *component, CstNodeProps *props) {
  sys_return_if_fail(jnode->type == AstJPair);

  JPair *pair = jnode->v.v_pair;
  sys_return_if_fail(component != NULL);
  sys_return_if_fail(props != NULL);

  AstNodePass pass = { 0 };
  pass.v_component = component;
  pass.v_props = props;

  if (pair->prop != NULL) {

    ast_iter_jobject(pair->prop, (AstJNodeFunc)node_parse_prop_func, &pass);
  }
}

/* Css */
CstCssPair *ast_css_pair_parse(JNode *jnode) {
  sys_return_val_if_fail(jnode->type == AstJPair, NULL);

  CstCssValueNode *node;
  CstCssValue *value;
  CstCssPair *pair = NULL;
  JPair *jpair = jnode->v.v_pair;
  SysChar *key = jpair->key;

  sys_return_val_if_fail(key != NULL, NULL);

  node = cst_css_value_node_lookup(key);
  if (node == NULL) {
    sys_warning_N("Failed to lookup css key: \"%s\"", key);
    return NULL;
  }

  value = cst_css_value_new(node);

  cst_css_value_parse(ast_jnode_jnode(jpair->value), value);

  pair = cst_css_pair_new(key, value);

  return pair;
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
  CstManager *manager = cst_module_get_manager(v_module);
  const SysChar *comp_id;

  CstComponentProps prop = { 0 };

  ast_component_property_parse(comp_ast->property, &prop);

  comp_id = prop.v_id;

  cst_manager_lock(manager);
  SysType type = cst_manager_get_meta(manager, comp_id);
  cst_manager_unlock(manager);

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

      cst_module_load_gstyle(v_module, gstyle);
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
SysInt ast_css_value_parse(JNode *jnode, CstCssValue *value) {
  sys_return_val_if_fail(value != NULL, SYS_FAILED);
  sys_return_val_if_fail(jnode != NULL, SYS_FAILED);

  SysInt *v_m4;

  CstCssValueNode *node = cst_css_value_get_node(value);

  switch (jnode->type) {
    case AstJBool:
      cst_css_value_set_bool(value, jnode->v.v_bool);
      break;
    case AstJSource:
    case AstJString:
      cst_css_value_set_string(value, jnode->v.v_string);
      break;
    case AstJNull:
      cst_css_value_set_null(value, NULL);
      break;
    case AstJArray:
      if (jnode->v.v_array->len != 4) {
        break;
      }

      v_m4 = sys_new0_N(SysInt, 4);
      for (SysUInt i = 0; i < jnode->v.v_array->len; i++) {
        JNode *node = jnode->v.v_array->pdata[i];

        node = ast_jnode_jnode(node);
        if (node->type != AstJInt) {
          sys_warning_N("%s", "css value array only should be int and length should be 4.");
          continue;
        }

        v_m4[i] = node->v.v_int;
      }

      cst_css_value_set_m4(value, v_m4);
      break;
    case AstJNode:
    case AstJProperty:
    case AstJObject:
    case AstJPair:
      cst_css_value_set_pointer(value, (SysPointer)jnode->v.v_node);
      break;
    case AstJInt:
      cst_css_value_set_int(value, jnode->v.v_int);
      break;
    case AstJDouble:
      cst_css_value_set_double(value, jnode->v.v_double);
      break;
    default:
      sys_warning_N("Failed to parse css value with key: %s", cst_css_value_node_name(node));
      break;
  }

  return SYS_SUCCESS;
}

SysInt ast_css_value_width_parse(JNode *v, CstCssValue *value) {
  CstCssClosure *c;

  if (v->type == AstJString) {
    c = cst_css_value_parse_calc(v->v.v_string, cst_css_value_width_percent);
    if (c == NULL) {
      sys_warning_N("Faild to parse width percent: %s", v->v.v_string);
      return SYS_FAILED;
    }

    cst_css_value_set_closure(value, c);

  } else if (v->type == AstJInt) {

    cst_css_value_set_int(value, v->v.v_int);
  } else {
    sys_warning_N("Cannot parse width type: %d.", v->type);

    return SYS_FAILED;
  }


  return SYS_SUCCESS;
}

SysInt ast_css_value_height_parse(JNode *v, CstCssValue *value) {
  CstCssClosure *c;

  if (v->type == AstJString) {
    c = cst_css_value_parse_calc(v->v.v_string, cst_css_value_height_percent);

    if (c == NULL) {
      sys_warning_N("Faild to parse height: %s", v->v.v_string);
      return SYS_FAILED;
    }

    cst_css_value_set_closure(value, c);

  } else if (v->type == AstJInt) {

    cst_css_value_set_int(value, v->v.v_int);

  } else {
    sys_warning_N("Cannot parse height type: %d.", v->type);

    return SYS_FAILED;
  }


  return SYS_SUCCESS;
}

SysInt ast_css_value_color_parse(JNode *v, CstCssValue *value) {
  sys_return_val_if_fail(v != NULL, SYS_FAILED);
  sys_return_val_if_fail(v->type == AstJString, SYS_FAILED);

  FRColor *color = sys_new0_N(FRColor, 1);

  if (!fr_color_rgba_parse(v->v.v_string, color)) {
    sys_warning_N("Faild to parse %s.", v->v.v_string);
    return SYS_FAILED;
  }

  cst_css_value_set_color(value, color);

  return SYS_SUCCESS;
}

/* CodeGen */
void ast_codegen_node_parse(CodeGen *cg, AstNode *node) {
#if 0
  sys_return_if_fail(node != NULL);

  SysChar *codename, *headername;
  SysChar* rtname;

  switch (node->type) {
    case AstRoot:
      {
        sys_debug_N("%s","[AstRoot]");
        cg->c_ast = ast_component_by_node(node);
        if (cg->c_ast == NULL) {
          sys_error_N("%s", "No component detected: %s", cg->filename);
          return;
        }

        cg->code_map = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, sys_free);
        cg->header_map = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, sys_free);

        rtname = sys_path_purename(cg->filename);
        codename = sys_strdup_printf("%s.c", rtname);
        headername = sys_strdup_printf("%s.h", rtname);

        cg->code_tio = sys_tio_new(codename);
        sys_tio_set_src(cg->code_tio, cg->filename);
        codegen_for_node_array(cg, node->v.root);

        cg->header_tio = sys_tio_new(headername);
        codegen_varmap_basic(cg->header_map, cg->c_ast->id);
        sys_tio_write_template(cg->header_tio, HEADER_TEMPLATE, cg->header_map);
        sys_hash_table_unref(cg->header_map);

        sys_free_N(rtname);
        sys_free_N(codename);
        sys_free_N(headername);

        sys_tio_free(cg->code_tio);
        sys_tio_free(cg->header_tio);
      }
      break;
    case AstImport:
      sys_debug_N("%s","[AstImport]");
      codegen_for_import(cg, node->v.import);
      break;
    case AstSource:
      {
        sys_debug_N("%s","[AstSource]");
        Source *src = node->v.source;

        codegen_varmap_basic(cg->code_map, cg->c_ast->id);
        sys_hash_table_insert(cg->code_map, "C_FUNCTION_CODE", sys_strdup(src->string));
        sys_hash_table_insert(cg->code_map, "C_HEADER_CODE", sys_strdup("test header code"));

        sys_tio_write_template(cg->code_tio, CODE_TEMPLATE, cg->code_map);

        sys_hash_table_unref(cg->code_map);
      }
      break;
    case AstComponent:
      sys_debug_N("%s","[AstComponent]");
      sys_debug_N("%s", node->v.component->id);

      codegen_for_jnode(cg, node->v.component->property);
      codegen_for_jnode(cg, node->v.component->body);
      break;
    case AstJsonNode:
      sys_debug_N("%s","[AstJsonNode]");
      break;
    case AstInvalid:
      sys_debug_N("%s","[AstInvalid]");
      break;
    default:
      break;
  }
#endif
}
