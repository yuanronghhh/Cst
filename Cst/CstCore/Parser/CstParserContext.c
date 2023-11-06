#include <CstCore/Parser/CstParserContext.h>


SYS_DEFINE_TYPE(CstParserContext, cst_parser_context, SYS_TYPE_OBJECT);


/* context */
SysBool cst_parser_context_realize(CstParserContext *c, AstNode *ast) {
  sys_return_val_if_fail(c != NULL, false);
  sys_return_val_if_fail(c->realize_func != NULL, false);

  return c->realize_func(ast, c->user_data);
}

SysBool cst_parser_context_import(CstParserContext *c, AstNode *ast) {
  sys_return_val_if_fail(c != NULL, false);
  sys_return_val_if_fail(c->import_func != NULL, false);

  return c->import_func(ast, c->user_data);
}

/* object api */
static void cst_parser_context_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(cst_parser_context_parent_class)->dispose(o);
}

CstParserContext *cst_parser_context_new(void) {
  return sys_object_new(CST_TYPE_PARSER_CONTEXT, NULL);
}

static void cst_parser_context_class_init(CstParserContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  ocls->dispose = cst_parser_context_dispose;
}

static void cst_parser_context_init(CstParserContext *self) {
}
