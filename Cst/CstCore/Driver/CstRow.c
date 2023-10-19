#include <CstCore/Driver/CstRow.h>

#define ROW_LIST(o) (o->parent)

CstRow *cst_row_add(CstRow *row, CstLayoutNode *lnode) {
  sys_slist_prepend(ROW_LIST(row), lnode);
}

static void cst_row_free(CstRow *o) {
  sys_free_N(o);
}

CstRow *cst_row_new(void) {
  CstRow *o = sys_new0_N(CstRow, 1);
  return o;
}

CstRow *cst_row_new_I(SysInt x, SysInt y) {
  CstRow *o = cst_row_new();

  o->x = x;
  o->y = y;

  return o;
}
