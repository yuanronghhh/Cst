#include <CstCore/Driver/FlexLayout/CstFlexLine.h>

SYS_DEFINE_TYPE(CstFlexLine, cst_flex_line, SYS_TYPE_OBJECT);


static SysObject *cst_flex_line_dclone_i(SysObject* o) {
  SysObject* n = SYS_OBJECT_CLASS(cst_flex_line_parent_class)->dclone(o);

  CstFlexLine* nself = CST_FLEX_LINE(n);
  CstFlexLine* oself = CST_FLEX_LINE(o);

  for (SysUInt i = 0; i < oself->items.len; i++) {
    CstIFlexItem* item = (CstIFlexItem *)sys_object_dclone(oself->items.pdata[i]);
    cst_flex_line_add(nself, item);
  }

  return n;
}

void cst_flex_line_add(CstFlexLine* self, CstIFlexItem *item) {
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


}

static void cst_flex_line_class_init(CstFlexLineClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_flex_line_dispose;
  ocls->dclone = cst_flex_line_dclone_i;
}

void cst_flex_line_init(CstFlexLine* self) {
  sys_harray_init_with_free_func(&self->items, (SysDestroyFunc)_sys_object_unref);
}

