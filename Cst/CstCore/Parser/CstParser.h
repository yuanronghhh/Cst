#ifndef __CST_PARSER_H__
#define __CST_PARSER_H__

#include <CstCore/Parser/CstParserCommon.h>

SYS_BEGIN_DECLS


SYS_API void cst_parser_set_data(Parser* ps, SysPointer user_data);
SYS_API SysPointer cst_parser_get_data(Parser* ps);
SYS_API AstNode* cst_parser_get_root(Parser* ps);
SYS_API void cst_parser_set_root(Parser* ps, AstNode* root);
SYS_API const SysChar* cst_parser_get_filename(Parser* ps);
SYS_API void cst_parser_error(Parser* ps, const SysChar* format, ...);
SYS_API Parser* cst_parser_new(const SysChar* fullpath);
SYS_API bool cst_parser_parse(Parser* ps);
SYS_API void cst_parser_free(Parser* ps, SysBool free_node);
SYS_API yyscan_t cst_parser_get_scanner(Parser* ps);

SYS_END_DECLS

#endif

