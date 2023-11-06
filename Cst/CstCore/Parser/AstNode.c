#include <CstCore/Parser/AstPrivate.h>


void ast_print_component(Component *c) {
  ast_print_jnode(c->property, NULL);
  ast_print_jnode(c->body, NULL);
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

void ast_print_visitor(AstNode *node, SysPointer user_data) {
  ast_print_node(node, user_data);
}

void ast_iter_narray(SysPtrArray *array, AstNodeFunc func, SysPointer user_data) {
  for (SysUInt i = 0; i < array->len; i++) {
    AstNode *node = array->pdata[i];

    if (!func(node, user_data)) {
      break;
    }
  }
}

AstNode* ast_for_root(JArray *array) {
  AstNode *node = sys_new0_N(AstNode, 1);

  node->type = AstRoot;
  node->v.root = array;

  return node;
}

AstNode* ast_for_import(SysPtrArray *array, SysChar *path) {
  sys_return_val_if_fail(path != NULL, NULL);

  AstNode *node = sys_new0_N(AstNode, 1);

  Import *import = sys_new0_N(Import, 1);
  import->ids = array;
  import->path = path;

  node->type = AstImport;
  node->v.import = import;

  return node;
}

AstNode* ast_for_gstyle(JNode* prop_node, JNode *body_node) {
  sys_return_val_if_fail(body_node != NULL, NULL);

  AstNode *node = sys_new0_N(AstNode, 1);
  GStyle *gstyle = sys_new0_N(GStyle, 1);

  node->type = AstGStyle;

  gstyle->property = prop_node;
  gstyle->body = body_node;

  node->v.gstyle = gstyle;

  return node;
}

AstNode* ast_for_component(JNode* prop_node, JNode *body_node) {
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

AstNode* ast_for_source(const SysChar *str, SysInt rows) {
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

AstNode *ast_root_get_gstyle(AstNode *root) {
  sys_return_val_if_fail(root != NULL, NULL);
  sys_return_val_if_fail(root->type == AstRoot, NULL);

  AstNode *nnode = NULL;
  ast_iter_narray(root->v.root, ast_is_gstyle, &nnode);
  if (nnode == NULL) {
    return NULL;
  }

  return nnode;
}

SysInt ast_node_get_type(AstNode *ast) {
  sys_return_val_if_fail(ast != NULL, -1);

  return ast->type;
}
