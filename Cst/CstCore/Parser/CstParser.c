#include <CstCore/Driver/CstContext.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Parser/Ast.h>
#include <CstCore/Parser/CstParser.h>

struct _Parser {
  yyscan_t scanner;
  YYSTYPE udata;
  AstNode* root_node;
  FILE* fp;
  SysChar* filename;
  SysPointer user_data;
};

void cst_parser_set_data(Parser* ps, SysPointer user_data) {
  ps->user_data = user_data;
}

SysPointer cst_parser_get_data(Parser* ps) {
  return ps->user_data;
}

Parser* cst_parser_new(const SysChar* fullpath) {
  Parser* ps = sys_new0_N(Parser, 1);

  FILE* fp = sys_fopen(fullpath, "r");
  if (fp == NULL) {
    sys_error_N("%s: %s\n", sys_strerror(errno), fullpath);
    goto fail;
  }

  ps->fp = fp;
  ps->filename = sys_strdup(fullpath);
  ps->scanner = NULL;

  if(yylex_init_extra(ps, &ps->scanner) != 0) {
    sys_abort_N("%s", "scanner init failed.");
    goto fail;
  }

  yyset_in(fp, ps->scanner);
  yyrestart(fp, ps->scanner);

  return ps;

fail:
  cst_parser_free(ps, true);
  return NULL;
}

void cst_parser_free(Parser* ps, SysBool free_node) {
  yylex_destroy(ps->scanner);

  if (free_node && ps->root_node) {
    ast_node_free(ps->root_node);
  }

  sys_fclose(ps->fp);
  sys_free_N(ps->filename);
  sys_free_N(ps);
}

void cst_parser_error(Parser* ps, const SysChar* format, ...) {
  cst_parser_free(ps, false);
}

void cst_parser_set_root(Parser* ps, AstNode* root) {
  ps->root_node = root;
}

AstNode* cst_parser_get_root(Parser* ps) {
  return ps->root_node;
}

const SysChar* cst_parser_get_filename(Parser* ps) {
  return ps->filename;
}

bool cst_parser_parse(Parser* ps) {
  sys_return_val_if_fail(ps != NULL, false);

  YYSTYPE* udata = &ps->udata;

  if (yyparse(ps, udata, ps->scanner) != 0) {
    return false;
  }

  sys_debug_N(SYS_("Parse successful: %s"), cst_parser_get_filename(ps));
  return true;
}

yyscan_t cst_parser_get_scanner(Parser* ps) {
  return ps->scanner;
}
