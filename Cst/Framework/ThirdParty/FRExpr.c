#include <Framework/ThirdParty/FRExpr.h>


struct _FRExprManager {
  SysSList *main;
  SysSList *delay;
};

static FRExprManager gexpr;

void fr_expr_init(void) {
}

double fr_expr_eval(const FRExpr *n) {
  return te_eval(n);
}

FRExpr *fr_expr_compile(const char *expression, const FRVariable *variables, int var_count, int *error) {
  return te_compile(expression, variables, var_count, error);
}

void fr_expr_array_push(FRExpr *expr) {
  gexpr.main  = sys_slist_append(gexpr.main, expr);
}

void fr_expr_array_delay_push(int *prop_pair, FRExpr *expr) {
  gexpr.delay = sys_slist_append(gexpr.delay, expr);
}

void fr_expr_eval_item(double *prop_pair, FRExpr *expr) {
  *prop_pair = te_eval(expr);
}

void fr_expr_array_eval() {
}

