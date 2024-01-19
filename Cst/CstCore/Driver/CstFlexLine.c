#include <CstCore/Driver/CstFlexLine.h>


CstFlexLine *cst_flex_line_new(void) {
  CstFlexLine *self = sys_new0_N(CstFlexLine, 1);

  sys_harray_init_with_free_func(&self->items, _sys_object_unref);

  return self;
}

void cst_flex_line_free(CstFlexLine *self) {
  sys_return_if_fail(self != NULL);

  sys_free_N(self);
}

void cst_flex_item_add(CstFlexLine*self, CstFlexItem *item) {
  sys_harray_add(&self->items, item);
}

CstFlexLine *cst_flex_line_dclone(const CstFlexLine* line) {
  CstFlexLine* nline = cst_flex_line_new();
  
  for (SysUInt i = 0; i < line->items.len; i++) {
    CstFlexItem* item = (CstFlexItem *)sys_object_dclone(line->items.pdata[i]);
    cst_flex_item_add(nline, item);
  }

  return nline;
}
