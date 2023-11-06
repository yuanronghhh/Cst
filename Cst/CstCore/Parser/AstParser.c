#include <CstCore/Parser/AstParser.h>

#include <CstCore/Parser/AstPrivate.h>
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


typedef struct _AstNodePass AstNodePass;

struct _AstNodePass {
  CstNode *pnode;
  AstParser *parser;
};


CstCssGroup* ast_css_group_new_with_jpair(FREnv *env, JPair *pair, SysBool key_lookup);
SYS_DEFINE_TYPE(AstParser, ast_parser, CST_TYPE_PARSER);

static SysValue *ast_parser_jnode_to_value(CstModule *v_module, const SysChar *key, JNode *jnode) {
  sys_return_val_if_fail(key != NULL, NULL);

  FREventFunc func;
  const SysChar *sv;
  const SysChar *mpath;
  SysValue *v;

  if(*key == '@' && ast_jnode_get_type(jnode) == AstJString) {
    sv = ast_jnode_get_string(jnode);
    mpath = cst_module_get_path(v_module);

    func = cst_module_get_event_function(v_module, sv);
    if (func == NULL) {
      sys_warning_N("Not found function: \"%s\" in \"%s\" module", sv, mpath);
      return NULL;
    }

    v = sys_value_new();
    sys_value_set_v_pointer(v, func);
    return v;

  }

  return ast_jnode_to_value(jnode);
}

CstModule * ast_parser_get_v_module(AstParser *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_module;
}

CstComponent * ast_parser_get_v_component(AstParser *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_component;
}

/* CstComNode */
static SysBool com_node_parse_prop_func(JNode *jnode, AstParser *self) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(self != NULL, false);

  JNode *nnode;
  CstNodeMap *map;
  CstValueMap *prop_map;
  SysValue *svalue;
  SysInt ptype;

  CstComNodeBuilder *builder = CST_COM_NODE_BUILDER(self->node_builder);
  sys_return_val_if_fail(builder != NULL, false);

  CstModule *v_module = self->v_module;
  sys_return_val_if_fail(v_module != NULL, false);

  JPair *pair = ast_jnode_jpair(jnode);
  nnode = ast_jnode_jnode(pair->value);
  if (nnode == NULL) {
    return false;
  }

  ptype = cst_node_prop_get_by_name(pair->key);
  if(ptype == -1) {

    sys_warning_N("failed to parse node property: %s", pair->key);
    return false;
  }

  svalue = ast_parser_jnode_to_value(v_module, pair->key, nnode);
  prop_map = cst_value_map_new_I(pair->key, sys_value_get_data_type(svalue));

  map = cst_node_map_new_I(prop_map, CST_NODE_PROP_ACTION, pair->key, svalue);
  sys_object_unref(prop_map);

  cst_com_node_builder_set_node_map(builder, map);

  return true;
}

void ast_com_node_parse(AstParser *self, JNode *jnode) {
  sys_return_if_fail(jnode->type == AstJPair);

  JPair *pair = jnode->v.v_pair;
  sys_return_if_fail(pair != NULL);

  if (pair->prop != NULL) {

    ast_iter_jobject(pair->prop, (AstJNodeFunc)com_node_parse_prop_func, self);
  }
}

/* Component */
JNode* ast_component_body_value(JNode *node) {
  sys_return_val_if_fail(node != NULL, NULL);

  return ast_jpair_value_value(node);
}

static SysBool component_style_node_func(JNode *jnode, AstParser *self) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(jnode->type == AstJPair, false);
  JPair *pair = jnode->v.v_pair;

  CstComponentBuilder *builder = self->comp_builder;
  sys_return_val_if_fail(builder != NULL, false);

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

static SysBool ast_component_parse_layout_func(JNode *jnode, AstNodePass *pass) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(pass != NULL, false);
  sys_return_val_if_fail(ast_jnode_get_type(jnode) == AstJPair, false);

  JNode *njnode;
  CstNode *v_node;
  SysType type;
  SysInt count;
  const SysChar *cus_name;
  CstNodeBuilder *builder;
  SysChar* tname;

  JPair *pair = jnode->v.v_pair;
  sys_return_val_if_fail(pair != NULL, false);

  AstParser *self = pass->parser;
  sys_return_val_if_fail(self != NULL, false);

  CstModule *v_module = self->v_module;
  sys_return_val_if_fail(v_module != NULL, false);

  CstNode *pnode = pass->pnode;
  sys_return_val_if_fail(pnode != NULL, false);

  CstComponent *v_component = self->v_component;
  sys_return_val_if_fail(v_component != NULL, false);

  cus_name = pair->key;

  CstComponent *child_comp = cst_module_get_component(v_module, cus_name);
  if (child_comp != NULL) {
    builder = cst_com_node_builder_new();
    v_node = cst_com_node_new();

    tname = sys_strdup_printf("<%s>", cst_component_get_id(v_component));
    cst_node_set_name(v_node, tname);
    sys_free_N(tname);
  } else {
    type = cst_render_node_get_meta(cus_name);
    if (type == 0) {
      sys_error_N("Not found node or component \"%s\".", cus_name);
      return false;
    }

    builder = cst_node_builder_new();
    v_node = cst_node_new();

    cst_node_set_name(v_node, cus_name);
    cst_node_set_rnode_type(v_node, type);
  }

  cst_node_builder_parse(builder, self, jnode);
  cst_node_builder_build(builder, self, v_node);

  sys_object_unref(builder);

  count = cst_module_get_count(v_module);
  cst_module_set_count(v_module, ++count);

  fr_node_append(FR_NODE(pnode), FR_NODE(v_node));

  njnode = ast_jnode_jnode(pair->value);
  if (ast_jnode_get_type(njnode) == AstJNull) {
    return true;

  } else if (ast_jnode_get_type(njnode) ==  AstJObject) {
    AstNodePass npass = {0};
    npass.parser = self;
    npass.pnode = v_node;

    ast_iter_jobject(njnode, (AstJNodeFunc)ast_component_parse_layout_func, &npass);

  } else {

    sys_warning_N("Not correct type in component \"%s\" pair value: %s", cus_name, pair->key);
  }

  return true;
}

static SysBool component_body_func(JNode *pair, AstParser *self) {
  sys_return_val_if_fail(pair != NULL, false);
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
      {
        body_node = ast_component_body_value(pair);
        ast_iter_jobject(body_node, (AstJNodeFunc)ast_component_parse_layout_func, self);
      }
      break;

    case CST_COMPONENT_BCOMPONENT:
    case CST_COMPONENT_BPROPS:
      body_node = ast_component_body_value(pair);

      ast_iter_jobject(body_node, (AstJNodeFunc)ast_component_parse_props_func, self->comp_builder);
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
  JPair *pair;

  pair = jnode->v.v_pair;
  nnode = ast_jnode_jnode(pair->value);
  if (nnode == NULL) { return false; }

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

static void ast_component_property_parse(JNode *jnode, CstComponentBuilder *builder) {

  ast_iter_jobject(jnode, (AstJNodeFunc)component_property_parse_func, builder);
}

void ast_component_body_parse(JNode *jnode, AstParser *self) {

  ast_iter_jobject(jnode, (AstJNodeFunc)component_body_func, self);
}

void ast_parser_parse_component(AstParser *self, AstNode *node) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(node->type == AstComponent);

  const SysChar *comp_id;
  CstComponent *v_component;
  Component *comp_ast;
  SysType type;

  CstComponentBuilder *builder = cst_component_builder_new();
  sys_return_if_fail(builder != NULL);

  CstModule *v_module = self->v_module;
  sys_return_if_fail(builder != NULL);
  self->comp_builder = builder;

  comp_ast = node->v.component;
  ast_component_property_parse(comp_ast->property, builder);

  comp_id = cst_component_builder_get_id(builder);
  if(comp_id == NULL) {
    sys_error_N("%s", "Not found id in component, maybe not init types ?");
    return;
  }

  type = cst_render_node_get_meta(comp_id);
  if (type == 0) {
    sys_error_N("Not found %s in component, maybe not init types ?", comp_id);
    return;
  }

  v_component = sys_object_new(type, NULL);
  if (v_component == NULL) {
    sys_error_N("Failed to create component: %s", comp_id);
    return;
  }
  self->v_component = v_component;

  ast_component_body_parse(comp_ast->body, self);
  cst_component_builder_build(builder, v_component);

  cst_module_set_root_component(v_module, v_component);
  sys_object_unref(builder);
}

/* Import */
static SysBool ast_parser_parse_import_array(AstParser *self, SysPtrArray *sarray, const SysChar *path) {
  sys_return_val_if_fail(self != NULL, false);
  const SysChar *id;
  CstModule *child;
  CstComponent *comp;

  child = cst_module_load_path(self->pnode, self->v_module, path);
  if (child == NULL) {
    sys_error_N("import module failed: %s", path);
    abort();
  }

  if (sarray->len > 0) {
    comp = cst_module_get_root_component(child);

    for (SysUInt i = 0; i < sarray->len; i++) {
      id = sarray->pdata[i];

      if (comp) {

        cst_module_set_component(self->v_module, (SysPointer)id, comp);
      }
    }
  }

  return true;
}

static void parse_import_handle(AstNode * node, AstParser *self) {
  sys_return_if_fail(node->type == AstImport);
  Import *import = node->v.import;

  const SysChar *mpath;
  SysChar *fullpath;
  SysChar *cwd;

  mpath = cst_module_get_path(self->v_module);
  cwd = sys_path_dirname(mpath);
  fullpath= sys_path_join(cwd, import->path);

  sys_free_N(cwd);

  if (fullpath == NULL || !sys_path_exists(fullpath)) {
    sys_error_N("import path not found: %s", fullpath);
    abort();
  }

  if (!ast_parser_parse_import_array(self, import->ids, fullpath)) {
    abort();
  }

  sys_free_N(fullpath);
}

/* GStyle Ast */
static SysBool ast_css_env_gstyle_func(JNode *node, AstParser *self) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(node != NULL, false);
  sys_return_val_if_fail(node->type == AstJPair, false);

  const SysChar *id;
  FREnv *gcss_env;
  JPair *jpair;
  CstCssGroup *g;
  const SysChar *mpath;

  gcss_env = self->gcss_env;
  jpair = node->v.v_pair;

  if(self->v_module) {
    mpath = cst_module_get_path(self->v_module);
  } else {
    mpath = "global";
  }

  g = ast_css_group_new_with_jpair(gcss_env, jpair, true);
  if (g == NULL) {
    sys_abort_N("parse css failed: %s", mpath);
  }

  id = cst_css_group_get_id(g);
  fr_env_set(gcss_env, id, g);

  return true;
}

void ast_parser_parse_gstyle(AstParser *self, AstNode *node) {
  sys_return_if_fail(node != NULL);
  sys_return_if_fail(node->type == AstGStyle);

  self->gcss_env = cst_css_env_get_gcss_env();

  GStyle* gstyle = node->v.gstyle;
  sys_return_if_fail(gstyle != NULL);

  ast_iter_jobject(gstyle->body, (AstJNodeFunc)ast_css_env_gstyle_func, self);
}

void ast_parser_root_gstyle_handle(AstNode *node, AstParser *self) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(node != NULL);

  AstNode *nnode = ast_root_get_gstyle(node);

  ast_parser_parse_gstyle(self, nnode);
}

/* Node Ast */
static SysBool ast_node_parse_value_bind(
    AstParser *ctx,
    CstNodeBuilder *builder,
    const SysChar *key, const SysChar *expr_str) {
  sys_return_val_if_fail(expr_str != NULL, false);

  CstComponent *v_component = ctx->v_component;
  sys_return_val_if_fail(v_component != NULL, false);

  CstValueMap *vmap = NULL;
  CstNodeMap *map;
  SysChar *index_name;
  SysInt len = (SysInt)sys_strlen(expr_str, 100);

  index_name = cst_node_builder_extract_index(expr_str, len);
  if (index_name == NULL) {
    return false;
  }

  vmap = cst_component_get_value_map(v_component, index_name);
  sys_free_N(index_name);

  if (vmap == NULL) {
    return false;
  }

  map = cst_node_map_new_I(vmap, CST_NODE_PROP_VALUE, key, NULL);

  cst_node_builder_add_nodemap(builder, map);
  cst_node_builder_set_v_value(builder, expr_str);

  return true;
}

static SysBool ast_node_parse_action_bind(AstParser *ctx, CstNodeBuilder *builder, const SysChar *watch_name, const SysChar *func_name, SysChar **bind_var) {
  sys_return_val_if_fail(func_name != NULL, false);

  CstValueMap *pmap = NULL;
  CstNodeMap *map;
  SysChar *index_name;
  SysInt len;

  CstComponent *v_component = ctx->v_component;
  sys_return_val_if_fail(v_component != NULL, false);

  len = (SysInt)sys_strlen(func_name, 100);
  index_name = cst_node_builder_extract_index(func_name, len);
  if (index_name == NULL) {
    return false;
  }
  *bind_var = index_name;

  pmap = cst_component_get_value_map(v_component, index_name);
  if (pmap == NULL) {
    sys_error_N("Not found props in component: %s, %s", cst_component_get_id(v_component), index_name);
    *bind_var = NULL;
    sys_free_N(index_name);
    return false;
  }

  map = cst_node_map_new_I(pmap, CST_NODE_PROP_BIND, watch_name, NULL);
  cst_node_builder_add_nodemap(builder, map);

  return true;
}

SysBool ast_node_parse_action(
    AstParser *ctx,
    CstNodeBuilder *nbuilder,
    const SysChar *watch_name,
    const SysChar *func_name) {

  sys_return_val_if_fail(func_name != NULL, false);
  sys_return_val_if_fail(watch_name != NULL, false);

  SysChar *fname;
  FRAWatchBuilder* builder;
  FRAWatch *awatch = NULL;
  FREventFunc watch_func = NULL;
  SysChar *bind_var = NULL;
  SysType type;

  CstModule *v_module = ctx->v_module;
  sys_return_val_if_fail(v_module != NULL, false);

  CstComponent *v_component = ctx->v_component;
  sys_return_val_if_fail(v_component != NULL, false);

  if(*func_name == '{') {
    if(!ast_node_parse_action_bind(ctx, nbuilder, watch_name, func_name, &bind_var)) {
      return false;
    }

  } else {

    fname = sys_strdup_printf("%s%s", FR_FUNC_EVENT_PREFIX, func_name);
    watch_func = (FREventFunc)cst_module_get_function(v_module, fname);
    sys_free_N(fname);

    if (watch_func == NULL) {
      sys_warning_N("Not found function: \"%s\" in \"%s\" component",
        func_name, cst_component_get_id(v_component));
      return false;
    }

    bind_var = sys_strdup(func_name);
  }

  type = fr_awatch_get_type_by_name(watch_name);
  sys_clear_pointer(&bind_var, sys_free);

  if (type == 0) {
    sys_warning_N("Not found watch: %s,%s", watch_name, func_name);
    goto fail;
  }

  awatch = sys_object_new(type, NULL);

  builder = fr_awatch_builder_new_I(func_name, watch_func);
  fr_awatch_construct(awatch, builder);
  sys_object_unref(builder);

  if (awatch == NULL) {

    sys_warning_N("Not found action: \"%s\" in \"%s\" component", watch_name, cst_component_get_id(v_component));
    goto fail;
  }

  cst_node_builder_add_awatch(nbuilder, awatch);

  return true;

fail:
  if(awatch != NULL) {
    sys_clear_pointer(&awatch, _sys_object_unref);
  }

  if(bind_var != NULL) {
    sys_free_N(bind_var);
  }
  return false;
}

SysBool ast_node_parse_layer_name(CstNodeBuilder *builder, const SysChar *pstr) {
  sys_return_val_if_fail(builder != NULL, false);
  sys_return_val_if_fail(pstr != NULL, false);

  SysInt layer_type = cst_node_layer_by_name(pstr);
  if(layer_type == -1) {
    sys_warning_N("node builder layer_type not correct: %s", pstr);
    return false;
  }

  return cst_node_builder_set_layer(builder, layer_type);
}

static SysBool ast_string_array_flatten (SysPtrArray *v_array, const SysChar *key, SysChar ***o_base, SysUInt *o_len) {
  JNode *tnode;
  SysChar **base = sys_new0_N(SysChar *, v_array->len);

  for (SysUInt i = 0; i < v_array->len; i++) {
    tnode = v_array->pdata[i];
    if (tnode == NULL || tnode->type != AstJNode) {
      return false;
    }

    tnode = tnode->v.v_node;

    if (tnode->type != AstJString) {
      sys_warning_N("Node property must be a string: %s", key);
      sys_free_N(base);
      return false;
    }

    base[i] = tnode->v.v_string;
  }

  *o_base = base;
  *o_len = v_array->len;

  return true;
}

SysBool ast_node_parse_base(AstParser *ctx, CstNodeBuilder *builder, const SysChar *base[], SysUInt len) {
  sys_return_val_if_fail(builder != NULL, false);
  sys_return_val_if_fail(base != NULL, false);

  SysPtrArray *css_list;

  CstComponent *v_component = ctx->v_component;
  sys_return_val_if_fail(v_component != NULL, false);

  css_list = cst_component_parse_base(v_component, base, len);
  cst_node_builder_set_v_css_list(builder, css_list);

  return true;
}

static SysBool node_parse_prop_func(JNode *jnode, AstParser *self) {
  sys_return_val_if_fail(jnode != NULL, false);
  sys_return_val_if_fail(self != NULL, false);

  JNode *nnode;
  CST_NODE_PROP_ENUM prop;
  JPair *pair = jnode->v.v_pair;
  SysChar **o_base;
  SysUInt o_len;

  CstNodeBuilder *builder = self->node_builder;
  sys_return_val_if_fail(builder != NULL, false);

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

      o_base = NULL;
      o_len = 0;

      if(ast_string_array_flatten(nnode->v.v_array, pair->key, &o_base, &o_len)) {
        return false;
      }

      if(!ast_node_parse_base(self, builder, (const SysChar **)o_base, o_len)) {
        return false;
      }

      sys_free_N(o_base);

      break;
    case CST_NODE_PROP_VALUE:
      if (nnode->type != AstJString) {
        return false;
      }

      if(!ast_node_parse_value_bind(self, builder, pair->key, (const SysChar *)nnode->v.v_string)) {
        return false;
      }
      break;
    case CST_NODE_PROP_POSITION:
      if (nnode->type != AstJString) {
        return false;
      }

      if(!ast_node_parse_layer_name(builder, nnode->v.v_string)) {
        return false;
      }
      break;
    case CST_NODE_PROP_ACTION:
      if (nnode->type != AstJString) {
        return false;
      }

      if(!ast_node_parse_action(self, builder, pair->key + 1, nnode->v.v_string)) {
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

void ast_node_props_parse(AstParser *self, JNode *jnode) {
  sys_return_if_fail(jnode != NULL);
  sys_return_if_fail(jnode->type == AstJPair);

  JPair *pair = jnode->v.v_pair;

  if (pair->prop != NULL) {

    ast_iter_jobject(pair->prop, (AstJNodeFunc)node_parse_prop_func, self);
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

  node = cst_css_node_get_g_css_node(key);
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
SysBool ast_module_body_func(AstParser *self, AstNode *ast) {
  sys_return_val_if_fail(ast != NULL, false);

  switch(ast->type) {
    case AstImport:
      break;

    case AstComponent:
      {
        ast_parser_parse_component(self, ast);
      }
      break;

    case AstGStyle:
      {
        ast_parser_parse_gstyle(self, ast);
      }
      break;

    case AstSource:
      break;

    default:
      sys_error_N("Not support module root type: %s", ast->type);
      break;
  }

  return true;
}

static void parse_module_handle(AstNode *node, AstParser *self) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(node != NULL);

  ast_iter_narray(node->v.root, (AstNodeFunc)ast_module_body_func, self);
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

      sys_warning_N("Failed to parse css value with key: %s", cst_css_node_get_name(css_node));
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

/* object api */
static void ast_parser_construct_i(CstParser *o, FILE *fp, const SysChar *fullpath) {

  CST_PARSER_CLASS(ast_parser_parent_class)->construct(o, fp, fullpath);
}

static void ast_parser_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(ast_parser_parent_class)->dispose(o);
}

CstParser *ast_parser_new(void) {
  return sys_object_new(AST_TYPE_PARSER, NULL);
}

CstParser *ast_parser_new_I(const SysChar *path, CstModule *v_module, CstNode *pnode) {
  FILE *fp = sys_fopen(path, "r");
  if(fp == NULL) {
    return NULL;
  }

  CstParser *o = ast_parser_new();
  AstParser *self = AST_PARSER(o);

  ast_parser_construct_i(o, fp, path);

  self->v_module = v_module;

  return o;
}

static void ast_parser_class_init(AstParserClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstParserClass *pcls = CST_PARSER_CLASS(cls);

  ocls->dispose = ast_parser_dispose;
  pcls->construct = ast_parser_construct_i;
}

static void ast_parser_init(AstParser *self) {
}
