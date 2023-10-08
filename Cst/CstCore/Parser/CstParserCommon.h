#ifndef __PARSER_COMMON_H__
#define __PARSER_COMMON_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define YYPTRDIFF_T SysSize
#define YYPTRDIFF_MAXIMUM LONG_MAX
#define YY_TYPEDEF_YY_SCANNER_T
#define yyscan_t void*

#define SAVE_VALUE(m, t)  yylval_param->m = t

// keyword can not be free
#define SAVE_STRING   SAVE_VALUE(v_string, sys_strdup(v_yytext))
#define SAVE_TOKEN(t) SAVE_VALUE(v_token, t)

#define SCONST(cstr) { cstr##_token, #cstr, (sizeof(#cstr) - 1) }
#define YY_EXTRA_TYPE CstParser*
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yyget_lineno(yyscanner); \
                    yylloc.first_column = yyget_column(yyscanner);  \
                    yylloc.last_column = yylloc.first_column + yyget_leng(yyscanner) - 1; \
                    yyset_column(yylloc.first_column, yyscanner);

/* ast.h */

#include <CstCore/Parser/CstCli.yy.h>
#define LEX_PARAMS_VALS ps, yylval_param, yyscanner
#define LEX_PARAMS CstParser *ps, YYSTYPE * yylval_param, yyscan_t yyscanner
#define YY_DECL int yylex(LEX_PARAMS)

int yylex(LEX_PARAMS);
int yylex_init_extra(YY_EXTRA_TYPE user_defined, yyscan_t* scanner);
int yylex_init(yyscan_t* scanner);
int yylex_destroy (yyscan_t yyscanner);
void yyrestart(FILE*  input_file, yyscan_t yyscanner);
void yyset_in(FILE*  fp, yyscan_t yyscanner);
void yyset_out(FILE*  fp, yyscan_t yyscanner);
char* yyget_text(yyscan_t yyscanner);
int yyget_leng(yyscan_t yyscanner);
void yyset_lineno(int  _line_number, yyscan_t yyscanner);
int yyget_lineno(yyscan_t yyscanner);
void yyset_column(int _column_no, yyscan_t yyscanner);
int yyget_column(yyscan_t yyscanner);
YY_EXTRA_TYPE yyget_extra(yyscan_t yyscanner);
void yyset_extra(YY_EXTRA_TYPE user_defined, yyscan_t yyscanner);

void yyerror(LEX_PARAMS, const char* msg);


SYS_END_DECLS

#endif

