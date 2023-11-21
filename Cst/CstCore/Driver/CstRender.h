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

  CstLayer *box_layer;
  CstLayer *abs_layer;
};

SysType cst_render_get_type(void);
CstRender* cst_render_new_I(SysBool is_offscreen);

void cst_render_setup(void);
void cst_render_teardown(void);
CstRender *cst_render_get_g_render(void);

FRWindow *cst_render_get_default_window(CstRender *self);
void cst_render_resize_window(CstRender *self);
void cst_render_request_resize_window(CstRender *self, SysInt width, SysInt height);
void cst_render_rerender(CstRender *self, FRRegion *region, CstLayout *layout);
CstLayer *cst_render_get_default_layer(void);

CstLayer *cst_render_get_layer_by_type(CstRender *self, SysInt layer_type);
void cst_render_render(CstRender *self, CstModule *v_module);

void cst_render_set_body_node(CstRender *self, CstNode * body_node);
CstNode * cst_render_get_body_node(CstRender *self);

SYS_END_DECLS

#endif

