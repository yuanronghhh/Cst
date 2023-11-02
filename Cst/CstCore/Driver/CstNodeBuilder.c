#include <CstCore/Driver/CstNodeBuilder.h>

#include <CstCore/Front/Common/CstText.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <Framework/Event/Action/FRAWatch.h>


SYS_DEFINE_TYPE(CstNodeBuilder, cst_node_builder, SYS_TYPE_OBJECT);


void cst_node_builder_build_i (CstNodeBuilder *self, CstNode *node) {
  CstNode *node = CST_NODE(o);
  sys_return_if_fail(self != NULL);

  SysChar *id;
  CstModule *v_module;

  v_module = cst_context_get_v_module(self->c);
  if (self->v_id) {

    cst_node_set_id(node, self->v_id);
  } else {

    id = cst_module_new_uid(v_module);

    cst_node_set_id(node, id);
    sys_free_N(id);
  }

  if(self->v_awatch_list) {
    cst_node_set_v_awatch_list(node, self->v_awatch_list);
  }

  if(self->v_nodemap_list) {
    cst_node_set_v_nodemap_list(node, self->v_nodemap_list);
  }

  if(self->v_css_list) {
    cst_node_set_v_css_list(node, self->v_css_list);
  }

  if (self->v_value) {
    cst_node_set_v_value(node, self->v_value);
  }

  if (self->v_label) {

    cst_node_set_v_label(node, self->v_label);
  }

  cst_node_set_v_z_index(node, self->v_z_index);
}

void cst_node_builder_build_text(CstNodeBuilder *self, CstRenderNode *rnode) {
  CstText* text = CST_TEXT(rnode);

  if (self->v_value) {

    cst_text_set_text(text, self->v_value);
  }
}

CstNode* cst_node_builder_get_pnode(CstNodeBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_pnode;
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

SysBool cst_node_builder_set_layer(CstNodeBuilder *self, SysInt v_layer) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(v_layer > 0, false);

  self->v_layer = v_layer;

  return true;
}

void cst_node_builder_set_label(CstNodeBuilder *self, const SysChar *v_label) {
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

SysList * cst_node_builder_get_nodemap_list(CstNodeBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_nodemap_list;
}

SysChar* cst_node_builder_extract_index(const SysChar* str, SysInt slen) {
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

SysBool cst_node_builder_awatch_name(CstNodeBuilder *self, const SysChar *name, const SysChar *func_name) {
  sys_return_val_if_fail(self != NULL, false);

  SysType type = fr_awatch_get_type_by_name(name);
  if (type == 0) {
    sys_warning_N("Not found watch: %s,%s", name, func_name);
    return false;
  }

  return true;
}

void cst_node_builder_add_awatch(CstNodeBuilder *self, FRAWatch* map) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(map != NULL);

  self->v_awatch_list = sys_list_prepend(self->v_awatch_list, map);
}

SysList * cst_node_builder_get_awatch_list(CstNodeBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_awatch_list;
}

/* parse */
SysBool cst_node_builder_parse_layer_name(CstNodeBuilder *self, const SysChar *pstr) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(pstr != NULL, false);

  SysInt layer_type = cst_node_layer_by_name(pstr);
  if(layer_type == -1) {
    sys_warning_N("node builder layer_type not correct: %s", pstr);
    return false;
  }

  return cst_node_builder_set_layer(self, layer_type);
}

SysBool cst_node_builder_parse_value_bind(CstNodeBuilder *self, const SysChar *key, const SysChar *expr_str) {
  sys_return_val_if_fail(expr_str != NULL, false);
  sys_return_val_if_fail(self != NULL, false);

  CstValueMap *vmap = NULL;
  CstNodeMap *map;
  SysChar *index_name;
  SysInt len = (SysInt)sys_strlen(expr_str, 100);

  CstComponent *v_component = cst_context_get_v_component(self->c);
  sys_return_val_if_fail(v_component != NULL, false);

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

  cst_node_builder_add_nodemap(self, map);
  cst_node_builder_set_v_value(self, expr_str);

  return true;
}

static SysBool node_builder_parse_action_bind(CstNodeBuilder *self, const SysChar *watch_name, const SysChar *func_name, SysChar **bind_var) {
  sys_return_val_if_fail(func_name != NULL, false);
  sys_return_val_if_fail(self->c != NULL, false);

  CstValueMap *pmap = NULL;
  CstNodeMap *map;
  SysChar *index_name;
  SysInt len;

  CstComponent *v_component = cst_context_get_v_component(self->c);

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
  cst_node_builder_add_nodemap(self, map);

  return true;
}

SysBool cst_node_builder_parse_action(CstNodeBuilder *self, const SysChar *watch_name, const SysChar *func_name) {
  sys_return_val_if_fail(func_name != NULL, false);
  sys_return_val_if_fail(watch_name != NULL, false);

  SysChar *fname;
  FRAWatch *awatch = NULL;
  FREventFunc watch_func = NULL;
  SysChar *bind_var = NULL;
  FRAWatchBuilder* builder;

  CstModule *v_module = cst_context_get_v_module(self->c);
  sys_return_val_if_fail(v_module != NULL, false);

  CstComponent *v_component = cst_context_get_v_component(self->c);
  sys_return_val_if_fail(v_component != NULL, false);

  if(*func_name == '{') {
    if(!node_builder_parse_action_bind(self, watch_name, func_name, &bind_var)) {
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

  SysType type = fr_awatch_get_type_by_name(watch_name);
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

  cst_node_builder_add_awatch(self, awatch);

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

/* object api */
static void cst_node_builder_dispose(SysObject* o) {
  CstNodeBuilder *self = CST_NODE_BUILDER(o);

  if(self->v_id) {
    sys_clear_pointer(&self->v_id, sys_free);
  }

  if (self->v_tag) {
    sys_clear_pointer(&self->v_tag, sys_free);
  }

  if(self->v_value) {
    sys_clear_pointer(&self->v_value, sys_free);
  }

  if(self->v_label) {
    sys_clear_pointer(&self->v_label, sys_free);
  }

  SYS_OBJECT_CLASS(cst_node_builder_parent_class)->dispose(o);
}

CstBuilder *cst_node_builder_new(void) {
  return sys_object_new(CST_TYPE_NODE_BUILDER, NULL);
}

void cst_node_builder_construct(CstNodeBuilder *o, CstContext *c, CstNode* v_pnode) {
  CstNodeBuilder *self = CST_NODE_BUILDER(o);

  self->c = c;
  self->v_pnode = v_pnode;
  self->v_layer = CST_NODE_LAYER_BOX;
}

CstBuilder *cst_node_builder_new_I(CstContext *c, CstNode* v_pnode) {
  CstBuilder *o = cst_node_builder_new();

  cst_node_builder_construct(o, c, v_pnode);

  return o;
}

static void cst_node_builder_class_init(CstNodeBuilderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_node_builder_dispose;
  cls->construct = cst_node_builder_construct;
  cls->build = cst_node_builder_build_i;
}

static void cst_node_builder_init(CstNodeBuilder *self) {
}
