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


void cst_node_builder_build_text(CstNodeBuilder *self, CstRenderNode *rnode) {
  CstText* text = CST_TEXT(rnode);

  if (self->v_value) {

    cst_text_set_text(text, self->v_value);
  }
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

void cst_node_builder_set_v_css_list(CstNodeBuilder *self, SysPtrArray * v_css_list) {
  sys_return_if_fail(self != NULL);

  self->v_css_list = v_css_list;
}

SysPtrArray * cst_node_builder_get_v_css_list(CstNodeBuilder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v_css_list;
}

/* parse */
void cst_node_builder_parse(CstNodeBuilder *self, AstParser *c, JNode *jnode) {
  sys_return_if_fail(self != NULL);

  CstNodeBuilderClass* lcls = CST_NODE_BUILDER_GET_CLASS(self);
  sys_return_if_fail(lcls->parse != NULL);

  lcls->parse(self, c, jnode);
}

void cst_node_builder_build(CstNodeBuilder *self, AstParser *c, CstNode *node) {
  sys_return_if_fail(self != NULL);

  CstNodeBuilderClass* lcls = CST_NODE_BUILDER_GET_CLASS(self);
  sys_return_if_fail(lcls->build != NULL);

  lcls->build(self, c, node);
}

static void cst_node_builder_build_i(CstNodeBuilder *self, AstParser *c, CstNode *node) {
  sys_return_if_fail(self != NULL);

  SysChar *id;
  CstModule *v_module;

  v_module = ast_parser_get_v_module(c);
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

static void cst_node_builder_parse_i (CstNodeBuilder *self, AstParser *c, JNode *jnode) {

  ast_node_props_parse(c, jnode);
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

CstNodeBuilder *cst_node_builder_new(void) {
  return sys_object_new(CST_TYPE_NODE_BUILDER, NULL);
}

static void cst_node_builder_class_init(CstNodeBuilderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_node_builder_dispose;
  cls->parse = cst_node_builder_parse_i;
  cls->build = cst_node_builder_build_i;
}

static void cst_node_builder_init(CstNodeBuilder *self) {
  self->v_layer = CST_NODE_LAYER_BOX;
}
