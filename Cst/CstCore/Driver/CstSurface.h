#ifndef __CST_SURFACE_H__
#define __CST_SURFACE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_SURFACE (cst_surface_get_type())
#define CST_SURFACE(o) ((CstSurface* )sys_object_cast_check(o, CST_TYPE_SURFACE))
#define CST_SURFACE_CLASS(o) ((CstSurfaceClass *)sys_class_cast_check(o, CST_TYPE_SURFACE)
#define CST_SURFACE_GET_CLASS(o) sys_instance_get_class(o, CstSurfaceClass)

struct _CstSurfaceClass {
  SysObjectClass parent;
};

struct _CstSurface {
  SysObject parent;

  /* <private> */
  FRSurface* surface;
  CstLayer* box_layer;
  CstLayer* abs_layer;
};

SysType cst_surface_get_type(void);
CstSurface *cst_surface_new(void);

CstSurface *cst_surface_new_I(FRSurface *fsurf);
CstLayer* cst_surface_get_layer_by_type(CstSurface* self, SysInt layer_type);
void cst_surface_layout_r(CstSurface* self, CstRenderNode* rnode, CstLayout* layout);
CstSurface* cst_surface_create_by_window(FRWindow *window, SysInt width, SysInt height);
CstSurface* cst_surface_create_image_surface(SysInt width, SysInt height);

SYS_END_DECLS

#endif
