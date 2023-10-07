#ifndef __CST_LAYOUT__
#define __CST_LAYOUT__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_LAYOUT (cst_layout_get_type())
#define CST_LAYOUT(o) ((CstLayout* )sys_object_cast_check(o, CST_TYPE_LAYOUT))
#define CST_LAYOUT_CLASS(o) ((CstLayoutClass *)sys_class_cast_check(o, CST_TYPE_LAYOUT))
#define CST_LAYOUT_GET_CLASS(o) sys_instance_get_class(o, CstLayoutClass)

struct _CstLayout {
  SysObject parent;

  /* <private> */
  int state;
  FRRegion *region;
  FRContext *cr;
};

struct _CstLayoutClass {
  SysObjectClass parent;
};

SysType cst_layout_get_type(void);
CstLayout *cst_layout_new(void);
CstLayout *cst_layout_new_I(FRContext *cr, FRRegion *region);

int cst_layout_get_state(CstLayout *self);
void cst_layout_set_state(CstLayout *self, int state);
SysBool cst_layout_state_layout(CstLayout *self);

FRRegion *cst_layout_get_region(CstLayout* self);
FRContext *cst_layout_get_cr(CstLayout* self);

void cst_layout_layout_box(CstLayout* self, FRDraw *draw, CstRenderNode *render_node);

SYS_END_DECLS

#endif

