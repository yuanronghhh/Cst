#ifndef __CST_PARSER_TYPES_H__
#define __CST_PARSER_TYPES_H__

#include <CstCore/CstConfig.h>
#include <Framework/FRCore.h>

SYS_BEGIN_DECLS

typedef enum _AstNodeType {
  AstRoot,
  AstImport,
  AstSource,
  AstComponent,
  AstGStyle,

  AstJsonNode,
  AstInvalid
} AstNodeType;

typedef enum _AstJNodeType {
  AstJBool = 1,
  AstJString,
  AstJArray,
  AstJObject,
  AstJNull,
  AstJSource,

  AstJInt,
  AstJDouble,
  AstJProperty,
  AstJPair,
  AstJNode,
  AstJPointer,

  AstJInvalid
} JNodeType;

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

typedef struct _CstParserContext CstParserContext;
typedef struct _CstParserContextClass CstParserContextClass;

typedef struct _AstParser AstParser;
typedef struct _AstParserClass AstParserClass;

typedef struct _CodeGen CodeGen;

typedef SysBool (*AstNodeFunc) (AstNode *ast, SysPointer user_data);
typedef SysBool (*AstJNodeFunc) (JNode *jnode, SysPointer user_data);
typedef SysInt (*AstJObjectValueFunc) (SysChar *key, JNodeType nodeType, SysPointer value);


SYS_END_DECLS

#endif

