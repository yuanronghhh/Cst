#ifndef __FR_EXPR_H__
#define __FR_EXPR_H__

#include <Framework/FRCommon.h>
#include "tinyexpr.h"

SYS_BEGIN_DECLS

typedef te_expr FRExpr;
typedef te_variable FRVariable;
typedef struct _FRExprManager FRExprManager;

SYS_API void fr_expr_array_push(FRExpr *expr);
SYS_API FRExpr *fr_expr_compile(const char *expression, const FRVariable *variables, int var_count, int *error);
SYS_API double fr_expr_eval(const FRExpr *n);
SYS_API void fr_expr_init(void);

SYS_END_DECLS

#endif

