#ifndef __CST_PARSER_H__
#define __CST_PARSER_H__

#include <CstCore/Parser/CstParserContext.h>

SYS_BEGIN_DECLS


#define CST_TYPE_PARSER (cst_parser_get_type())
#define CST_PARSER(o) ((CstParser* )sys_object_cast_check(o, CST_TYPE_PARSER))
#define CST_PARSER_CLASS(o) ((CstParserClass *)sys_class_cast_check(o, CST_TYPE_PARSER))
#define CST_PARSER_GET_CLASS(o) sys_instance_get_class(o, CstParserClass)


struct _CstParser {
  SysObject parent;

  /* < private > */
  yyscan_t scanner;
  YYSTYPE udata;
  FILE* fp;
  SysChar* filename;

  CstParserContext *ctx;
};

struct _CstParserClass {
  SysObjectClass parent;

  void (*construct) (CstParser *self, FILE *fp, const SysChar *fullpath);
};

SysType cst_parser_get_type(void);
CstParser *cst_parser_new(void);
CstParser *cst_parser_new_I(const SysChar *fullpath);

SysBool cst_parser_parse(CstParser* self);
const SysChar* cst_parser_get_filename(CstParser* self);
void cst_parser_error(CstParser* self, const SysChar* format, ...);
yyscan_t cst_parser_get_scanner(CstParser* self);

void cst_parser_set_ctx(CstParser *self, CstParserContext * ctx);
CstParserContext * cst_parser_get_ctx(CstParser *self);

SYS_END_DECLS

#endif

