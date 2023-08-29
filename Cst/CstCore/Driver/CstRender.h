#ifndef __CST_RENDER_H__
#define __CST_RENDER_H__

#include <CstCore/Driver/CstBoxLayer.h>
#include <CstCore/Driver/CstAbsLayer.h>

SYS_BEGIN_DECLS

#define CST_TYPE_RENDER (cst_render_get_type())
#define CST_RENDER(o) ((CstRender* )sys_object_cast_check(o, CST_TYPE_RENDER))
#define CST_RENDER_CLASS(o) ((CstRenderClass *)sys_class_cast_check(o, CST_TYPE_RENDER))
#define CST_RENDER_GET_CLASS(o) sys_instance_get_class(o, CstRenderClass)

struct _CstRenderClass {
  SysObjectClass parent;
};

struct _CstRender {
  SysObject parent;

  /* < private > */
  FRDisplay *display;
  FRWindow *window;

  FRDraw *draw;
  CstLayout *layout;

  CstLayer *box_layer;
  CstLayer *abs_layer;
};

SysType cst_render_get_type(void);
CstRender* cst_render_new_I(SysBool is_offscreen);

FRWindow *cst_render_get_default_window(CstRender *self);
void cst_render_resize_window(CstRender *self);
void cst_render_request_resize_window(CstRender *self, SysInt width, SysInt height);
void cst_render_rerender(CstRender *self, FRRegion *region);
void cst_render_render(CstRender *render);

void cst_render_set_node(CstRender * self, CstNode * parent, CstNode * node);
FRDraw *cst_render_get_draw(CstRender *self);
CstBoxLayer *cst_render_get_box_layer(CstRender *render);
CstAbsLayer *cst_render_get_abs_layer(CstRender *render);

SYS_END_DECLS

#endif

