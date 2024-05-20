#ifndef __FR_EXPR_H__
#define __FR_EXPR_H__

#include <Framework/FrCommon.h>
#include "tinyexpr.h"

SYS_BEGIN_DECLS

typedef te_expr FrExpr;
typedef te_variable FRVariable;
typedef struct _FrExprManager FrExprManager;

SYS_API void fr_expr_array_push(FrExpr *expr);
SYS_API FrExpr *fr_expr_compile(const char *expression, const FRVariable *variables, int var_count, int *error);
SYS_API double fr_expr_eval(const FrExpr *n);
SYS_API void fr_expr_init(void);

SYS_END_DECLS

#endif

