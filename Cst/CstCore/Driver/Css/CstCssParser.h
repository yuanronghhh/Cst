#ifndef __CST_CSS_PARSER_H__
#define __CST_CSS_PARSER_H__

#include <CstCore/Parser/Ast.h>

SYS_BEGIN_DECLS


#define CST_TYPE_CSS_PARSER (cst_css_parser_get_type())
#define CST_CSS_PARSER(o) ((CstCssParser* )sys_object_cast_check(o, CST_TYPE_CSS_PARSER))
#define CST_CSS_PARSER_CLASS(o) ((CstCssParserClass *)sys_class_cast_check(o, CST_TYPE_CSS_PARSER))
#define CST_CSS_PARSER_GET_CLASS(o) sys_instance_get_class(o, CstCssParserClass)

struct _CstCssParser {
  SysObject parent;

  /* <private> */
  JNode* jnode;
};

struct _CstCssParserClass {
  SysObjectClass parent;
};

SYS_API SysType cst_css_parser_get_type(void);
SYS_API CstCssParser* cst_css_parser_new(void);
SYS_API CstCssParser* cst_css_parser_new_I(JNode* jnode);

SYS_API SysBool cst_css_parser_parse_color(CstCssParser *parser, FRColor *color);
SYS_API CstCssClosure* cst_css_parser_parse_calc(CstCssParser* parser, SysChar* str, CstRenderNodeFunc func);

SYS_END_DECLS

#endif


