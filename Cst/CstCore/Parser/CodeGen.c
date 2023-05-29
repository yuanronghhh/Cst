#include <CstCore/Driver/CstManager.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Parser/Ast.h>
#include <CstCore/Parser/CodeGen.h>

/**
 * codegen: code generator for CstCli when create component file.
 * */

#define CODE_TEMPLATE CST_PROJECT_DIR"/Cst/CstCore/Config/ComponentCode.code"
#define HEADER_TEMPLATE CST_PROJECT_DIR"/Cst/CstCore/Config/ComponentCode.header"

struct _CodeGen {
  CstModule *module;
  CstManager *manager;

  SysTextIO *code_tio;
  SysTextIO *header_tio;
  SysHashTable *code_map;
  SysHashTable *header_map;
  Component *c_ast;

  SysChar *cwd;
  SysChar *filename;
};

void codegen_free(CodeGen *cg) {
  sys_free_N(cg->filename);
  sys_free_N(cg->cwd);
  sys_free_N(cg);
}

CodeGen *codegen_new(CstManager *manager, CstModule *module) {
  sys_return_val_if_fail(manager != NULL, NULL);
  sys_return_val_if_fail(module != NULL, NULL);

  CodeGen *cg = sys_new_N(CodeGen, 1);
  SysChar *mname;

  cg->manager = manager;
  cg->module = module;

  mname = cst_module_get_path(module);
  cg->cwd = sys_path_dirname(mname);
  cg->filename = sys_strdup(mname);

  return cg;
}

static void codegen_for_node_array(CodeGen *cg, SysPtrArray *array) {
  for(SysUInt i = 0; i < array->len; i++) {
    AstNode *node = array->pdata[i];
    ast_codegen_node_parse(cg, node);
  }
}

void codegen_for_jnode(CodeGen *cg, JNode *jnode) {
}

static void codegen_varmap_basic(SysHashTable *var_map, const SysChar *cname) {
  sys_return_if_fail(var_map != NULL);
  sys_return_if_fail(cname != NULL);

  SysChar *s, *name;
  SysInt plen, slen;

  s = strchr(cname, '-');
  if (s == NULL) {
    sys_error_N("component-name format should like <prefix>-<name>.", cname);
    return;
  }
  plen = (SysInt)(s - cname);
  slen = (SysInt)strlen(cname);

  name = sys_strdup(cname);

  *name = toupper(*name);
  *(name+plen+1) = toupper(*(name+plen+1));
  memmove(name + plen, name + plen + 1, slen - plen);
  sys_hash_table_insert(var_map, "TypeName", name);

  name = sys_strdup(cname);
  *(name + plen) = '_';
  sys_hash_table_insert(var_map, "type_name", name);

  name = sys_strdup(cname);
  name = sys_strupper(name);
  *(name + plen) = '_';
  sys_hash_table_insert(var_map, "TYPE_NAME", name);
}

void codegen_visitor(CodeGen *cg, AstNode *node) {
  ast_codegen_node_parse(cg, node);
}
