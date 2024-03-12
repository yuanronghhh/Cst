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

SYS_API SysType cst_surface_get_type(void);
SYS_API CstSurface *cst_surface_new(void);

SYS_API CstSurface* cst_surface_new_I(FRSurface* surface);
CstLayer* cst_surface_get_layer_by_type(CstSurface* self, SysInt layer_type);
void cst_surface_layout_r(CstSurface* self, CstRenderNode* rnode, CstLayout* layout);

SYS_END_DECLS

#endif
