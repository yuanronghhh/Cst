#include <CstCore/Driver/Flex/CstFlexLine.h>

SYS_DEFINE_TYPE(CstFlexLine, cst_flex_line, SYS_TYPE_OBJECT);


static CstFlexLine *cst_flex_line_dclone_i(const CstFlexLine* line) {
  CstFlexLine* nline = cst_flex_line_new();

  for (SysUInt i = 0; i < line->items.len; i++) {
    CstFlexItem* item = (CstFlexItem *)sys_object_dclone(line->items.pdata[i]);
    cst_flex_item_add(nline, item);
  }

  return nline;
}

void cst_flex_item_add(CstFlexLine* self, CstFlexItem *item) {
  sys_harray_add(&self->items, item);
}

/* object api */
static void cst_flex_line_construct(CstFlexLine *self) {

}

CstFlexLine* cst_flex_line_new(void) {
  return sys_object_new(CST_TYPE_FLEX_LINE, NULL);
}

CstFlexLine *cst_flex_line_new_I(void) {
  CstFlexLine *o = cst_flex_line_new();

  cst_flex_line_construct(o);

  return o;
}

static void cst_flex_line_dispose(SysObject* o) {
  // CstFlexLine *self = CST_FLEX_LINE(o);

  SYS_OBJECT_CLASS(cst_flex_line_parent_class)->dispose(o);
}

static void cst_flex_line_class_init(CstFlexLineClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_flex_line_dispose;
  ocls->dclone = (SysCloneFunc)cst_flex_line_dclone_i;
}

void cst_flex_line_init(CstFlexLine* self) {
  sys_harray_init_with_free_func(&self->items, (SysDestroyFunc)_sys_object_unref);
}

