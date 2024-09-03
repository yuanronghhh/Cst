#include <CstCore/Driver/CstNodeBuilder.h>

#include <CstCore/Front/CstText.h>
#include <CstCore/Driver/CstComNode.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/Css/CstCssEnv.h>


SYS_DEFINE_TYPE(CstNodeBuilder, cst_node_builder, SYS_TYPE_OBJECT);


void cst_node_builder_build_text(CstNodeBuilder *self, CstRenderNode *rnode) {
  CstText* text = CST_TEXT(rnode);

  if (self->v_value) {

    cst_text_set_text(text, self->v_value);
  }
}

SysChar* cst_builder_node_extract_index(const SysChar* str, SysInt slen) {
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

  nsp = sys_new0(SysChar, slen - 3);
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

SysBool cst_node_builder_parse_layer_name(CstNodeBuilder* o, const SysChar* pstr) {
  sys_return_val_if_fail(o != NULL, false);
  sys_return_val_if_fail(pstr != NULL, false);

  SysInt layer_idx = cst_layer_get_by_prop(pstr);
  if (layer_idx == -1) {
    sys_warning_N("node o layer_idx not correct: %s", pstr);
    return false;
  }
  o->v_layer_index = layer_idx;

  return true;
}

SysBool cst_node_builder_parse_base(CstNodeBuilder* o, 
  CstComponent* v_component, 
  const SysChar* base[], 
  SysUInt len) {
  sys_return_val_if_fail(o != NULL, false);
  sys_return_val_if_fail(v_component != NULL, false);

  SysHArray* css_list;

  css_list = cst_component_parse_base(v_component, base, len);
  o->v_css_list = css_list;

  return true;
}

SysBool cst_node_builder_parse_value_bind(
  CstNodeBuilder* o,
  CstComponent* v_component,
  const SysChar* key, 
  const SysChar* expr_str) {
  sys_return_val_if_fail(expr_str != NULL, false);

  CstValueMap* vmap = NULL;
  CstNodeMap* map;
  SysChar* index_name;
  SysInt len = (SysInt)sys_strlen(expr_str, 100);

  index_name = cst_builder_node_extract_index(expr_str, len);
  if (index_name == NULL) {
    return false;
  }

  vmap = cst_component_get_value_map(v_component, index_name);
  sys_free(index_name);

  if (vmap == NULL) {
    return false;
  }

  map = cst_node_map_new_I(vmap, CST_NODE_PROP_VALUE, key, NULL);

  cst_node_builder_add_nodemap(o, map);
  cst_node_builder_set_v_value(o, expr_str);

  return true;
}

SysBool cst_node_builder_parse_action_bind(
  CstNodeBuilder* o,
  CstComponent* comp,
  const SysChar* watch_name,
  const SysChar* func_name,
  SysChar** bind_var) {
  sys_return_val_if_fail(func_name != NULL, false);

  CstValueMap* pmap = NULL;
  CstNodeMap* map;
  SysChar* index_name;
  SysInt len;

  len = (SysInt)sys_strlen(func_name, 100);
  index_name = cst_builder_node_extract_index(func_name, len);
  if (index_name == NULL) {
    return false;
  }
  *bind_var = index_name;

  pmap = cst_component_get_value_map(comp, index_name);
  if (pmap == NULL) {
    sys_error_N("Not found props in component: %s, %s", cst_component_get_id(comp), index_name);
    *bind_var = NULL;
    sys_free(index_name);
    return false;
  }

  map = cst_node_map_new_I(pmap, CST_NODE_PROP_BIND, watch_name, NULL);
  cst_node_builder_add_nodemap(o, map);

  return true;
}

SysBool cst_node_builder_parse_action(
  CstNodeBuilder* bnode,
  CstModule* v_module,
  CstComponent* v_component,
  const SysChar* watch_name,
  const SysChar* func_name) {

  sys_return_val_if_fail(func_name != NULL, false);
  sys_return_val_if_fail(watch_name != NULL, false);

  SysChar* fname;
  FrAWatch* awatch = NULL;
  FrEventFunc watch_func = NULL;
  SysChar* bind_var = NULL;
  SysType type;

  if (*func_name == '{') {
    if (!cst_node_builder_parse_action_bind(bnode, v_component, watch_name, func_name, &bind_var)) {
      return false;
    }

  }
  else {

    fname = sys_strdup_printf("%s%s", FR_FUNC_EVENT_PREFIX, func_name);
    watch_func = (FrEventFunc)cst_module_get_function(v_module, fname);
    sys_free(fname);

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

  awatch = fr_awatch_new_by_type(type, func_name, watch_func);
  if (awatch == NULL) {

    sys_warning_N("Not found action: \"%s\" in \"%s\" component", watch_name, cst_component_get_id(v_component));
    goto fail;
  }

  cst_node_builder_add_awatch(bnode, awatch);

  return true;

fail:
  if (awatch != NULL) {
    sys_clear_pointer(&awatch, _sys_object_unref);
  }

  if (bind_var != NULL) {
    sys_free(bind_var);
  }
  return false;
}

const SysChar* cst_node_builder_get_value(CstNodeBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_value;
}

void cst_node_builder_set_id(CstNodeBuilder *self, const SysChar *v_id) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_id != NULL);

  sys_assert(self->v_id == NULL);

  self->v_id = sys_strdup(v_id);
}

void cst_node_builder_set_v_value(CstNodeBuilder *self, const SysChar *v_value) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_value != NULL);

  sys_assert(self->v_value == NULL);

  self->v_value = sys_strdup(v_value);
}

void cst_node_builder_set_v_label(CstNodeBuilder *self, const SysChar *v_label) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_label != NULL);

  sys_assert(self->v_label == NULL);

  self->v_label = sys_strdup(v_label);
}

void cst_node_builder_add_nodemap(CstNodeBuilder *self, CstNodeMap* map) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(map != NULL);

  self->v_nodemap_list = sys_list_prepend(self->v_nodemap_list, map);
}

void cst_node_builder_add_awatch(CstNodeBuilder *self, FrAWatch* map) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(map != NULL);

  self->v_awatch_list = sys_list_prepend(self->v_awatch_list, map);
}

void cst_node_builder_build_node(CstNodeBuilder *self, CstNode *node) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->v_name != NULL);

  CstModule *v_module = self->v_module;
  CstComponent *v_component = self->v_component;
  FrEnv *env = cst_component_get_css_env(v_component);
  CstCssGroup* g;

  cst_node_set_name(node, self->v_name);

  if (self->v_id == NULL) {

    self->v_id = cst_module_new_node_id(v_module);
  }
  cst_node_set_id(node, self->v_id);

  if (self->v_layer_index > 0) {

    cst_node_set_v_layer_idx(node, self->v_layer_index);
  }

  cst_node_set_v_awatch_list(node, self->v_awatch_list);
  self->v_awatch_list = NULL;

  cst_node_set_v_nodemap_list(node, self->v_nodemap_list);
  self->v_nodemap_list = NULL;

  /* set css */
  g = cst_css_group_get_by_id(env, self->v_id);
  if (g != NULL) {

    cst_css_group_set_r(self->v_css_list, g);
  }

  if (*self->v_name != '<') {
    g = cst_css_group_get_by_id(cst_css_env_get_gcss_env(), self->v_name);
    if (g == NULL) {

      sys_warning_N("set system css failed, config maybe not correct: %s",
          self->v_name);
    }

    cst_css_group_set_r(self->v_css_list, g);
  }
  cst_node_set_v_css_list(node, self->v_css_list);

  self->v_css_list = NULL;
}

void cst_node_builder_build_com_node(CstNodeBuilder *self, CstComNode *cnode) {
  sys_return_if_fail(self != NULL);
  CstNode *node = CST_NODE(cnode);
  CstComponent* comp = cst_com_node_get_component(cnode);

  self->v_name = sys_strdup_printf("<%s>", cst_component_get_id(comp));
  cst_node_builder_build_node(self, node);
}

/* object api */
static void cst_node_builder_dispose(SysObject* o) {
  CstNodeBuilder *self = CST_NODE_BUILDER(o);

  if (self->v_awatch_list) {

    sys_list_free_full(self->v_awatch_list, (SysDestroyFunc)_sys_object_unref);
  }

  if (self->v_nodemap_list) {

    sys_list_free_full(self->v_nodemap_list, (SysDestroyFunc)_sys_object_unref);
  }

  if (self->v_css_list) {
    
    sys_clear_pointer(&self->v_css_list, sys_harray_unref);
  }

  if (self->v_tag) {
    sys_clear_pointer(&self->v_tag, sys_free);
  }

  if(self->v_layer_index) {

    sys_clear_pointer(&self->v_layer_index, _sys_object_unref);
  }

  if (self->v_value) {
    sys_clear_pointer(&self->v_value, sys_free);
  }

  if(self->v_label) {
    sys_clear_pointer(&self->v_label, sys_free);
  }

  sys_clear_pointer(&self->v_id, sys_free);
  sys_clear_pointer(&self->v_name, sys_free);

  SYS_OBJECT_CLASS(cst_node_builder_parent_class)->dispose(o);
}

CstNodeBuilder *cst_node_builder_new(void) {
  return sys_object_new(CST_TYPE_NODE_BUILDER, NULL);
}

static void cst_node_builder_class_init(CstNodeBuilderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_node_builder_dispose;
}

static void cst_node_builder_init(CstNodeBuilder *self) {
  self->v_css_list = cst_css_group_list_new();
}
