#ifndef __FR_DRAW_H__
#define __FR_DRAW_H__

#include <Framework/FrCommon.h>


SYS_BEGIN_DECLS


#define FR_TYPE_DRAW (fr_draw_get_type())
#define FR_DRAW(o) ((FrDraw* )sys_object_cast_check(o, FR_TYPE_DRAW))
#define FR_DRAW_CLASS(o) ((FrDrawClass *)sys_class_cast_check(o, FR_TYPE_DRAW))
#define FR_DRAW_GET_CLASS(o) sys_instance_get_class(o, FrDrawClass)


struct _FrDrawClass {
  SysObjectClass parent;
};

struct _FrDraw {
  SysObject parent;

  /* <private> */
  FrContext *cr;
  FrSurface *window_surface;
  FrSurface *paint_surface;
  FrWindow* window;
  SysBool is_painting;
};

SYS_API SysType fr_draw_get_type(void);
SYS_API FrDraw* fr_draw_new_I(FrWindow *window);
SYS_API void fr_draw_frame_begin(FrDraw *self, FrRegion *region);
SYS_API void fr_draw_frame_end(FrDraw *self, FrRegion *region);
SYS_API SysBool fr_draw_frame_need_draw(FrDraw *self);
SYS_API void fr_draw_stroke_mp(FrDraw* self, const FrRect *bound, const FrSInt4* m4, const FrSInt4* p4);
SYS_API void fr_draw_fill_bound(FrDraw* self, const FrRect *bound);

SYS_API FrSurface* fr_draw_create_image_surface(SysInt width, SysInt height);
SYS_API void fr_draw_destroy_surface(FrSurface *surface);
SYS_API FrSurface* fr_draw_create_surface(FrWindow* window, SysInt width, SysInt height);
SYS_API FrSurface* fr_draw_create_image_surface_from_surface(FrSurface *surface, SysInt width, SysInt height);

/* text render */
SYS_API void fr_draw_draw_text(FrDraw* self, FrDrawLayout* layout, SysInt x, SysInt y);
SYS_API void fr_draw_show_text(FrDraw * self, FrDrawLayout* layout, SysInt x, SysInt y, SysInt m1, SysInt m0);
SYS_API void fr_draw_set_color(FrDraw *self, FrColor *color);
SYS_API void fr_draw_layout_layout(FrDraw * self, FrDrawLayout * layout);

/* context */
SYS_API void fr_draw_save(FrDraw* self);
SYS_API void fr_draw_restore(FrDraw* self);


SYS_END_DECLS

#endif

