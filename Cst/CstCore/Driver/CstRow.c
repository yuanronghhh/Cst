#include <CstCore/Driver/CstRow.h>

#define PARENT(o) (&(o)->parent)

const FRRect *cst_row_get_bound(CstRow* self) {
  sys_return_val_if_fail(self, NULL);

  return &self->bound;
}

void cst_row_add(CstRow *self, CstLayoutNode *lnode) {
  SysSList *list = PARENT(self);

  sys_slist_prepend(list, lnode);
}

static void cst_row_free(CstRow *self) {
  sys_free_N(self);
}

CstRow *cst_row_new(void) {
  CstRow *o = sys_new0_N(CstRow, 1);
  return o;
}

CstRow *cst_row_new_I(SysInt x, SysInt y) {
  CstRow *o = cst_row_new();

  o->bound.x = x;
  o->bound.y = y;

  return o;
}
