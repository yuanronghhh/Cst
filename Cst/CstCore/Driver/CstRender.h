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
  FrDisplay *display;
  FrIDevice *device;
  CstRenderNode *body_rnode;

  /* CstSurface */
  SysHArray surfaces;
};

SysType cst_render_get_type(void);
CstRender* cst_render_new_I(SysBool is_offscreen);

void cst_render_setup(void);
void cst_render_teardown(void);
CstRender *cst_render_get_g_render(void);

CstSurface *cst_render_get_surface(CstRender *self, SysUInt surf_idx);
FrIDevice *cst_render_get_default_device(CstRender *self);
void cst_render_resize_surface(CstRender *self);
void cst_render_request_resize_surface(CstRender *self, SysInt width, SysInt height);
void cst_render_rerender(CstRender* self, CstLayout* layout);
CstSurface* cst_render_get_default_surface(void);

void cst_render_render(CstRender *self, CstModule *v_module);

SYS_END_DECLS

#endif

