#ifndef __FR_DRAW_CONTEXT_H__
#define __FR_DRAW_CONTEXT_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_DRAW_CONTEXT (fr_draw_context_get_type())
#define FR_DRAW_CONTEXT(o) ((FrDrawContext* )sys_object_cast_check(o, FR_TYPE_DRAW_CONTEXT))
#define FR_DRAW_CONTEXT_CLASS(o) ((FrDrawContextClass *)sys_class_cast_check(o, FR_TYPE_DRAW_CONTEXT))
#define FR_DRAW_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, FrDrawContextClass)

struct _FrDrawContextClass {
  SysObjectClass parent;
};

struct _FrDrawContext {
  SysObject parent;

  /* <private> */
  FrIDraw *iface;
  FrSurface* idevice_surface;
  FrIDevice* idevice;

  /* FrSurface */
  SysHArray* surfaces;
  SysBool is_painting;
};

SYS_API SysType fr_draw_context_get_type(void);
SYS_API FrDrawContext *fr_draw_context_new(void);

SYS_API FrDrawContext* fr_draw_context_new_I(FrIDraw* iface, FrIDevice* idevice);
SYS_API SysBool fr_draw_context_frame_need_draw(FrDrawContext *self);
SYS_API void fr_draw_context_frame_begin(FrDrawContext *self, FrRegion *region);
SYS_API void fr_draw_context_frame_end(FrDrawContext *self, FrRegion *region);
SYS_API void fr_draw_context_get_buffer_size (FrDrawContext *self, SysInt *width, SysInt *height);
SYS_API FrSurface *fr_draw_context_get_surface_by_idx(FrDrawContext *self, SysInt idx);
SYS_API void fr_draw_context_set_surfaces(FrDrawContext *self, SysHArray * surfaces);
SYS_API SysHArray * fr_draw_context_get_surfaces(FrDrawContext *self);

void fr_draw_context_set_is_painting(FrDrawContext *self, SysBool is_painting);
SysBool fr_draw_context_get_is_painting(FrDrawContext *self);

SYS_END_DECLS

#endif
