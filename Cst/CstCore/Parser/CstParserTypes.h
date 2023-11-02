#ifndef __CST_PARSER_TYPES_H__
#define __CST_PARSER_TYPES_H__

#include <CstCore/CstConfig.h>
#include <Framework/FRCore.h>

SYS_BEGIN_DECLS

typedef struct _AstNode AstNode;
typedef struct _Source Source;
typedef struct _Component Component;
typedef struct _GStyle GStyle;
typedef struct _Import Import;
typedef struct _JNumber JNumber;

#define JBool bool
#define JDouble SysDouble
#define JInt SysInt
typedef struct _JPair JPair;
typedef struct _JNode JNode;
typedef SysPtrArray JArray;
typedef SysPtrArray JObject;
typedef SysChar JChar;
typedef Source JSource;
typedef SysPointer JPointer;

typedef struct _SConst SConst;
typedef struct _CstParser CstParser;
typedef struct _CstParserClass CstParserClass;

typedef struct _CodeGen CodeGen;

typedef struct _AstModulePass AstModulePass;
typedef struct _AstComponentPass AstComponentPass;
typedef struct _AstGStylePass AstGStylePass;
typedef struct _AstNodePass AstNodePass;
typedef struct _AstKeyNode AstKeyNode;
typedef SysBool (*AstNodeFunc) (AstNode *ast, SysPointer user_data);

SYS_END_DECLS

#endif

