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
  /* CST_RENDER_STATE_ENUM */
  SysInt state;

  /* CST_LAYER_ENUM */
  FrRegion* region;

  FrDrawContext *draw_context;

  /* CstSurface */
  SysHArray surfaces;
};

struct _CstLayoutClass {
  SysObjectClass parent;
};

SysType cst_layout_get_type(void);
CstLayout *cst_layout_new(void);
CstLayout *cst_layout_new_I(FrDrawContext *draw_context, FrRegion *region);
CstSurface* cst_layout_get_default_surface(CstLayout* self);
CstSurface* cst_layout_get_surface(CstLayout* self, SysUInt surf_idx);
void cst_layout_layout_root(CstLayout* self, CstRenderNode* rnode);
void cst_layout_paint_root(CstLayout* self, CstRenderNode* rnode);

void cst_layout_set_region(CstLayout *self, FrRegion * region);
FrRegion * cst_layout_get_region(CstLayout *self);

void cst_layout_set_state(CstLayout *self, SysInt state);
SysInt cst_layout_get_state(CstLayout *self);

SYS_END_DECLS

#endif

