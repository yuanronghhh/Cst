#include <CstCore/Parser/CstParser.h>
#include <CstCore/Parser/Ast.h>


SYS_DEFINE_TYPE(CstParser, cst_parser, SYS_TYPE_OBJECT);


void cst_parser_set_realize_func(CstParser *self, AstNodeFunc realize_func) {
  sys_return_if_fail(self != NULL);

  self->realize_func = realize_func;
}

void cst_parser_set_import_func(CstParser *self, AstNodeFunc import_func) {
  sys_return_if_fail(self != NULL);

  self->import_func = import_func;
}

void cst_parser_set_user_data(CstParser *self, SysPointer user_data) {
  sys_return_if_fail(self != NULL);

  self->user_data = user_data;
}

SysPointer cst_parser_get_user_data(CstParser *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->user_data;
}

SysBool cst_parser_realize(CstParser* self, AstNode *ast) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(ast != NULL, false);
  sys_return_val_if_fail(self->realize_func != NULL, false);

  SysBool r = self->realize_func(ast, self->user_data);
  ast_node_free(ast);

  return r;
}

SysBool cst_parser_import(CstParser* self, AstNode *ast) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(ast != NULL, false);
  sys_return_val_if_fail(self->import_func != NULL, false);

  return self->import_func(ast, self->user_data);
}

void cst_parser_error(CstParser* self, const SysChar* format, ...) {
  sys_return_if_fail(self != NULL);

  sys_object_unref(self);
}

const SysChar* cst_parser_get_filename(CstParser* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->filename;
}

SysBool cst_parser_parse(CstParser* self) {
  sys_return_val_if_fail(self != NULL, false);

  YYSTYPE* udata = &self->udata;

  if (yyparse(self, udata, self->scanner) != 0) {
    return false;
  }

  sys_debug_N(SYS_("Parse successful: %s"), cst_parser_get_filename(self));
  return true;
}

yyscan_t cst_parser_get_scanner(CstParser* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->scanner;
}

/* object api */
static void cst_parser_construct(CstParser *self, FILE *fp, const SysChar *fullpath) {
  self->fp = fp;
  self->filename = sys_strdup(fullpath);
  self->scanner = NULL;
}

static void cst_parser_dispose(SysObject* o) {
  CstParser *self = CST_PARSER(o);

  sys_clear_pointer(&self->scanner, yylex_destroy);
  sys_clear_pointer(&self->fp, sys_fclose);
  sys_clear_pointer(&self->filename, sys_free);

  SYS_OBJECT_CLASS(cst_parser_parent_class)->dispose(o);
}

CstParser *cst_parser_new(void) {
  return sys_object_new(CST_TYPE_PARSER, NULL);
}

CstParser *cst_parser_new_I(const SysChar *fullpath) {
  CstParser *self = cst_parser_new();

  FILE* fp = sys_fopen(fullpath, "r");
  if (fp == NULL) {
    sys_error_N("%s: %s\n", sys_strerror(errno), fullpath);
    goto fail;
  }

  cst_parser_construct(self, fp, fullpath);

  if(yylex_init_extra(self, &self->scanner) != 0) {
    sys_abort_N("%s", "scanner init failed.");
    goto fail;
  }

  yyset_in(fp, self->scanner);
  yyrestart(fp, self->scanner);

  return self;

fail:
  sys_object_unref(self);
  return NULL;
}

static void cst_parser_class_init(CstParserClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_parser_dispose;
}

static void cst_parser_init(CstParser *self) {
}
