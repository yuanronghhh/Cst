#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include <CstCore/Parser/CstParserCommon.h>

SYS_BEGIN_DECLS

SYS_API CodeGen *codegen_new(CstManager *n_manager, CstModule *n_module);
SYS_API void codegen_free(CodeGen *cg);
SYS_API void codegen_visitor(CodeGen *cg, AstNode *node);

SYS_END_DECLS

#endif

