#ifndef __CST_SURFACE_H__
#define __CST_SURFACE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_SURFACE (cst_surface_get_type())
#define CST_SURFACE(o) ((CstSurface* )sys_object_cast_check(o, CST_TYPE_SURFACE))
#define CST_SURFACE_CLASS(o) ((CstSurfaceClass *)sys_class_cast_check(o, CST_TYPE_SURFACE)
#define CST_SURFACE_GET_CLASS(o) sys_instance_get_class(o, CstSurfaceClass)

struct _CstSurfaceClass {
  FrSurfaceClass parent;
};

struct _CstSurface {
  FrSurface parent;

  /* <private> */

  /* paint surface */
  FrContext *cr;
  CstLayer* box_layer;
  CstLayer* abs_layer;
};

SysType cst_surface_get_type(void);
FrSurface* cst_surface_new(void);

FrSurface *cst_surface_new_I(FrSurfaceContext *info);
CstLayer* cst_surface_get_layer_by_type(CstSurface* self, SysInt layer_type);
FrSurface* cst_surface_create_device_surface(FrDevice *device, FrSurfaceContext *info);
FrSurface* cst_surface_create_image_surface(FrSurfaceContext *info);

void cst_surface_set_cr(CstSurface *self, FrContext * cr);
FrContext * cst_surface_get_cr(CstSurface *self);

SYS_END_DECLS

#endif
