#ifndef __AST_PARSER_H__
#define __AST_PARSER_H__

#include <CstCore/Parser/CstParser.h>


SYS_BEGIN_DECLS

#define AST_TYPE_PARSER (ast_parser_get_type())
#define AST_PARSER(o) ((AstParser* )sys_object_cast_check(o, AST_TYPE_PARSER))
#define AST_PARSER_CLASS(o) ((AstParserClass *)sys_class_cast_check(o, AST_TYPE_PARSER))
#define AST_PARSER_GET_CLASS(o) sys_instance_get_class(o, AstParserClass)

struct _AstParser {
  CstParser unowned;

  /* <private> */
  CstModule  *v_module;
  CstComponent  *v_component;
  CstComponent *v_pcomponent;
  CstComponent  *v_com_node;
  FREnv *gcss_env;
  CstNodeBuilder *node_builder;
  CstComponentBuilder *comp_builder;
  CstNode *pnode;
};

struct _AstParserClass {
  CstParserClass parent;
};

SysType ast_parser_get_type(void);
CstParser *ast_parser_new(void);
CstParser *ast_parser_new_I(const SysChar *path, CstModule *v_module, CstNode *pnode);

void ast_parser_parse_component(AstParser *self, AstNode *node);
void ast_parser_parse_gstyle(AstParser *self, AstNode *node);
void ast_parser_root_gstyle_handle(AstNode *node, AstParser *self);

void ast_node_props_parse(AstParser *self, JNode *jnode);
void ast_com_node_parse(AstParser *self, JNode *jnode);

SysBool ast_css_value_parse(JNode *jnode, CstCssNode *node, CstCssValue *value);
SysBool ast_css_value_color_parse(SysChar *s, CstCssValue *value);

CstComponent * ast_parser_get_v_component(AstParser *self);
CstModule * ast_parser_get_v_module(AstParser *self);

SYS_END_DECLS

#endif

