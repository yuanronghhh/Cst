#include <CstCore/Driver/FlexLayout/CstFlexBox.h>
#include <CstCore/Driver/FlexLayout/CstFlexLine.h>

SYS_DEFINE_TYPE(CstFlexBox, cst_flex_box, SYS_TYPE_OBJECT);


void layout_horizontal(CstFlexBox* self, CstLayout* layout, bool isRtl) {
  for (SysUInt i = 0; i < self->lines.len; i++) {
    CstFlexLine* line = self->lines.pdata[i];

    // measure

    // layout
    for(SysUInt j = 0; j < line->items.len; i++) {
    }
  }
}

void cst_flex_on_layout(CstFlexBox *self, CstLayout *layout) {
  SysInt layoutDirection = 0; // cst_layout_get_view_direction(layout);
  SysInt mFlexDirection = self->direction;
  SysBool isRtl = layoutDirection == mFlexDirection;

  switch (mFlexDirection) {
    case FLEX_DIRECTION_ROW:
      layout_horizontal(self, layout, isRtl);
      break;
    case FLEX_DIRECTION_ROW_REVERSE:
      layout_horizontal(self, layout, isRtl);
      break;
    case FLEX_DIRECTION_COLUMN:
      break;
    case FLEX_DIRECTION_COLUMN_REVERSE:
      break;
    default:
      sys_error_N("Invalid flex direction is set: %s", mFlexDirection);
  }
}

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

  SYS_OBJECT_CLASS(cst_flex_box_parent_class)->dispose(o);
}

static void cst_flex_box_class_init(CstFlexBoxClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_flex_box_dispose;
}

void cst_flex_box_init(CstFlexBox* self) {
}

