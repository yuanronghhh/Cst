#include <CstCore/Driver/FlexLayout/CstFlexBox.h>
#include <CstCore/Driver/FlexLayout/CstFlexLine.h>

SYS_DEFINE_TYPE(CstFlexBox, cst_flex_box, SYS_TYPE_OBJECT);

/* object api */
static void cst_flex_box_construct(CstFlexBox *self) {

}

CstFlexBox* cst_flex_box_new(void) {
  return sys_object_new(CST_TYPE_FLEX_BOX, NULL);
}

CstFlexBox *cst_flex_box_new_I(void) {
  CstFlexBox *o = cst_flex_box_new();

  cst_flex_box_construct(o);

  return o;
}

static void cst_flex_box_dispose(SysObject* o) {
  // CstFlexBox *self = CST_FLEX_BOX(o);


}

static void cst_flex_box_class_init(CstFlexBoxClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_flex_box_dispose;
}

void cst_flex_box_init(CstFlexBox* self) {
}

