#include <Framework/ThirdParty/FrExpr.h>


struct _FrExprManager {
  SysSList *main;
  SysSList *delay;
};

static FrExprManager gexpr;

void fr_expr_init(void) {
}

double fr_expr_eval(const FrExpr *n) {
  return te_eval(n);
}

FrExpr *fr_expr_compile(const char *expression, const FRVariable *variables, int var_count, int *error) {
  return te_compile(expression, variables, var_count, error);
}

void fr_expr_array_push(FrExpr *expr) {
  gexpr.main  = sys_slist_append(gexpr.main, expr);
}

void fr_expr_array_delay_push(int *prop_pair, FrExpr *expr) {
  gexpr.delay = sys_slist_append(gexpr.delay, expr);
}

void fr_expr_eval_item(double *prop_pair, FrExpr *expr) {
  *prop_pair = te_eval(expr);
}

void fr_expr_array_eval() {
}

