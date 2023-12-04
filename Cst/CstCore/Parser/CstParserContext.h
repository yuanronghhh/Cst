#ifndef __CST_PARSER_CONTEXT_H__
#define __CST_PARSER_CONTEXT_H__

#include <CstCore/Parser/CstParserCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_PARSER_CONTEXT (cst_parser_context_get_type())
#define CST_PARSER_CONTEXT(o) ((CstParserContext* )sys_object_cast_check(o, CST_TYPE_PARSER_CONTEXT))
#define CST_PARSER_CONTEXT_CLASS(o) ((CstParserContextClass *)sys_class_cast_check(o, CST_TYPE_PARSER_CONTEXT))
#define CST_PARSER_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, CstParserContextClass)

struct _CstParserContext {
  SysObject parent;
  SysPointer user_data;
  AstNodeFunc realize_func;
  AstNodeFunc import_func;

  /* <private> */
};

struct _CstParserContextClass {
  SysObjectClass parent;
};

SysType cst_parser_context_get_type(void);
CstParserContext *cst_parser_context_new(void);

SysBool cst_parser_context_import(CstParserContext *c, AstNode *ast);
SysBool cst_parser_context_realize(CstParserContext *c, AstNode *ast);

SYS_END_DECLS

#endif

