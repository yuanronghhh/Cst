#include <CstCore/Driver/Css/CstCssParser.h>
#include <CstCore/Driver/CstRenderNode.h>

SYS_DEFINE_TYPE(CstCssParser, cst_css_parser, SYS_TYPE_OBJECT);

SysBool cst_css_parser_parse_color(CstCssParser* parser, FRColor* color) {
  sys_return_val_if_fail(parser != NULL, false);

  //JNode* jnode = parser->jnode;
  //sys_return_val_if_fail(jnode->type != AstJString, false);

  //return fr_color_rgba_parse(parser->jnode->v.v_string, color);

  return false;
}

CstCssClosure *cst_css_parser_parse_calc(CstCssParser* parser, SysChar *str, CstCssCalcFunc func) {
  sys_return_val_if_fail(parser != NULL, NULL);

  return NULL;
}

/* object api */
static void cst_css_parser_dispose(SysObject* o) {
  CstCssParser *self = CST_CSS_PARSER(o);

  SYS_OBJECT_CLASS(cst_css_parser_parent_class)->dispose(o);
}

CstCssParser* cst_css_parser_new(void) {
  return sys_object_new(CST_TYPE_CSS_PARSER, NULL);
}

CstCssParser *cst_css_parser_new_I(JNode *jnode) {
  CstCssParser* o = cst_css_parser_new();

  o->jnode = jnode;

  return o;
}

static void cst_css_parser_class_init(CstCssParserClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_css_parser_dispose;
}

static void cst_css_parser_init(CstCssParser *self) {
}

void cst_css_parser_parse_value(JNode* jnode, CstCssValue* value) {
}

