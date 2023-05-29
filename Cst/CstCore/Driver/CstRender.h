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
  CstRenderPrivate *priv;
};

SYS_API SysType cst_render_get_type(void);
SYS_API CstRender* cst_render_new_I(SysBool is_offscreen);

SYS_API void cst_render_resize_window(CstRender *self);
SYS_API void cst_render_request_resize_window(CstRender *self, SysInt width, SysInt height);
SYS_API void cst_render_request_redraw(CstRender *self, FRRect *bound);
SYS_API void cst_render_queue_draw_node(CstRender * self, CstNode * node);

SYS_API FRContext *cst_render_get_cr(CstRender *render);
SYS_API void cst_render_set_node(CstRender * self, CstNode * parent, CstNode * node);
SYS_API FRRegion * cst_render_create_region(CstRender * self, const FRRect * n_rect);
SYS_API void cst_render_get_size(CstRender *render, SysInt *width, SysInt *height);
SYS_API CstBoxLayer *cst_render_get_box_layer(CstRender *render);
SYS_API CstAbsLayer *cst_render_get_abs_layer(CstRender *render);

SYS_API void cst_render_render (CstModule *v_module, CstRender *render);
SYS_API void cst_render_frame_begin (CstRender *self, FRRegion *region);
SYS_API void cst_render_frame_end (CstRender *self, FRRegion *region);

SYS_END_DECLS

#endif

