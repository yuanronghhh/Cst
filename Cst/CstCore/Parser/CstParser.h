#ifndef __CST_PARSER_H__
#define __CST_PARSER_H__

#include <CstCore/Parser/CstParserCommon.h>

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
  AstNode* root_node;
  FILE* fp;
  SysChar* filename;
  SysPointer user_data;
};

struct _CstParserClass {
  SysObjectClass parent;
};

SYS_API SysType cst_parser_get_type(void);
SYS_API CstParser *cst_parser_new(void);
SYS_API CstParser *cst_parser_new_I(const SysChar *fullpath);

SYS_API void cst_parser_set_root(CstParser* self, AstNode* root);

SYS_API void cst_parser_set_data(CstParser* self, SysPointer user_data);
SYS_API SysPointer cst_parser_get_data(CstParser* self);
SYS_API const SysChar* cst_parser_get_filename(CstParser* self);
SYS_API void cst_parser_error(CstParser* self, const SysChar* format, ...);
SYS_API bool cst_parser_parse(CstParser* self);
SYS_API yyscan_t cst_parser_get_scanner(CstParser* self);

SYS_API void cst_parser_module_parse(CstParser* self, CstModule* v_module);
SYS_API void cst_parser_gstyle_parse(CstParser* self, CstCssEnv* gcss_env);


SYS_END_DECLS

#endif

