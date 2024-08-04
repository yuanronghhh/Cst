#ifndef __FR_DRAW_CONTEXT_H__
#define __FR_DRAW_CONTEXT_H__

#include <Framework/FrCommon.h>
#include <Framework/Graph/FrIDraw.h>

SYS_BEGIN_DECLS

#define FR_TYPE_DRAW_CONTEXT (fr_draw_context_get_type())
#define FR_DRAW_CONTEXT(o) ((FrDrawContext* )sys_object_cast_check(o, FR_TYPE_DRAW_CONTEXT))
#define FR_DRAW_CONTEXT_CLASS(o) ((FrDrawContextClass *)sys_class_cast_check(o, FR_TYPE_DRAW_CONTEXT))
#define FR_DRAW_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, FrDrawContextClass)

struct _FrDrawContextClass {
  SysObjectClass parent;

  void (*construct) (FrDrawContext* self, FrDevice* device);
};

struct _FrDrawContext {
  SysObject parent;

  /* <private> */
  FrSurface* device_surface;
  FrDevice* device;

  SysHArray surfaces;
  SysBool is_painting;
};

void fr_draw_setup(void);
void fr_draw_teardown(void);

SYS_API SysType fr_draw_context_get_type(void);
SYS_API FrDrawContext *fr_draw_context_new(void);

SYS_API SysBool fr_draw_context_frame_need_draw(FrDrawContext *self);
SYS_API void fr_draw_context_frame_begin(FrDrawContext *self, FrRegion *region);
SYS_API void fr_draw_context_frame_end(FrDrawContext *self, FrRegion *region);
SYS_API void fr_draw_context_get_buffer_size (FrDrawContext *self,
    SysInt *width,
    SysInt *height);

SYS_API void fr_draw_context_construct(FrDrawContext* self, FrDevice* device);

SYS_API FrSurface* fr_draw_context_get_surface_by_idx(FrDrawContext* self,
    SysUInt idx);

SYS_API void fr_draw_context_add_surface(FrDrawContext* self, FrSurface *surface);
SYS_API SysHArray * fr_draw_context_get_surfaces(FrDrawContext *self);
SYS_API FrSurface* fr_draw_context_get_default_surface(FrDrawContext* self);

void fr_draw_context_set_is_painting(FrDrawContext *self, SysBool is_painting);
SysBool fr_draw_context_get_is_painting(FrDrawContext *self);

#define fr_draw_save fr_i_draw_save
#define fr_draw_restore fr_i_draw_restore
#define fr_draw_set_color fr_i_draw_set_color
#define fr_draw_context_render_video fr_i_draw_context_render_video

SYS_END_DECLS

#endif
