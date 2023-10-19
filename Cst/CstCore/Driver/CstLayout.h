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

  /* CST_RENDER_MODE_ENUM */
  SysInt mode;

  /* CST_RENDER_STAGE_ENUM */
  SysInt stage;

  /* CST_LAYER_ENUM */
  SysInt layer;
  FRRegion *region;
  FRDraw *draw;
};

struct _CstLayoutClass {
  SysObjectClass parent;
};

SysType cst_layout_get_type(void);
CstLayout *cst_layout_new(void);
CstLayout *cst_layout_new_I(FRDraw *cr, FRRegion *region);

SysBool cst_layout_is_state(CstLayout * self, SysInt state);
FRRegion *cst_layout_get_region(CstLayout* self);
FRDraw *cst_layout_get_draw(CstLayout* self);

void cst_layout_get_buffer_size(CstLayout* self, SysInt* width, SysInt* height);

void cst_layout_begin_layout(CstLayout* self, SysInt layer);
void cst_layout_end_layout(CstLayout * self);

void cst_layout_begin_node(CstLayout* self);
void cst_layout_end_node(CstLayout* self);

void cst_layout_set_mode(CstLayout *self, SysInt mode);
SysInt cst_layout_get_mode(CstLayout *self);

void cst_layout_set_stage(CstLayout *self, SysInt stage);
SysInt cst_layout_get_stage(CstLayout *self);

void cst_layout_set_state(CstLayout *self, SysInt state);
SysInt cst_layout_get_state(CstLayout *self);

void cst_layout_set_layer(CstLayout *self, SysInt layer);
SysInt cst_layout_get_layer(CstLayout *self);

SYS_END_DECLS

#endif

