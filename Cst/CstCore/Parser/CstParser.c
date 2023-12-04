#include <CstCore/Parser/CstParser.h>
#include <CstCore/Parser/Ast.h>


SYS_DEFINE_TYPE(CstParser, cst_parser, SYS_TYPE_OBJECT);

/* parser */
void cst_parser_set_ctx(CstParser *self, CstParserContext * ctx) {
  sys_return_if_fail(self != NULL);

  self->ctx = ctx;
}

CstParserContext * cst_parser_get_ctx(CstParser *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->ctx;
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

  if (yylex_init_extra(self, &self->scanner) != 0) {
    sys_abort_N("%s", "scanner init failed.");
  }

  yyset_in(fp, self->scanner);
  yyrestart(fp, self->scanner);
}

static void cst_parser_dispose(SysObject* o) {
  CstParser *self = CST_PARSER(o);

  sys_clear_pointer(&self->scanner, yylex_destroy);
  sys_clear_pointer(&self->fp, sys_fclose);
  sys_clear_pointer(&self->filename, sys_free);

  if (self->ctx) {

    sys_clear_pointer(&self->ctx, _sys_object_unref);
  }

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

  return self;

fail:
  sys_object_unref(self);
  return NULL;
}

static void cst_parser_class_init(CstParserClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_parser_dispose;
  cls->construct = cst_parser_construct;
}

static void cst_parser_init(CstParser *self) {
}
