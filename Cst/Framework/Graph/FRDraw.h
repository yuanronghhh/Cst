#ifndef __FR_DRAW_H__
#define __FR_DRAW_H__

#include <Framework/FRCommon.h>


SYS_BEGIN_DECLS


#define FR_TYPE_DRAW (fr_draw_get_type())
#define FR_DRAW(o) ((FRDraw* )sys_object_cast_check(o, FR_TYPE_DRAW))
#define FR_DRAW_CLASS(o) ((FRDrawClass *)sys_class_cast_check(o, FR_TYPE_DRAW))
#define FR_DRAW_GET_CLASS(o) sys_instance_get_class(o, FRDrawClass)


struct _FRDrawClass {
  SysObjectClass parent;
};

struct _FRDraw {
  SysObject parent;

  /* <private> */
  FRContext *cr;
  FRSurface *window_surface;
  FRSurface * paint_surface;
  FRWindow *window;
  SysBool is_painting;
};

SYS_API SysType fr_draw_get_type(void);
SYS_API FRDraw* fr_draw_new_I(FRWindow *window);
SYS_API void fr_draw_frame_begin(FRDraw *self, FRRegion *region);
SYS_API void fr_draw_frame_end(FRDraw *self, FRRegion *region);
SYS_API SysBool fr_draw_frame_need_draw(FRDraw *self);
SYS_API void fr_draw_get_size(FRDraw * self, SysInt * width, SysInt * height);
void fr_draw_stroke_mp(FRDraw* self, const FRRect *bound, const FRSInt4* m4, const FRSInt4* p4);
void fr_draw_fill_rectangle(FRDraw* self, const FRRect *bound);

/* text render */
void fr_draw_show_text(FRDraw * self, FRDrawLayout* layout, SysInt x, SysInt y, SysInt m1, SysInt m0);
void fr_draw_set_color(FRDraw *self, FRColor *color);
void fr_draw_layout_layout(FRDraw * self, FRDrawLayout * layout);

/* context */
void fr_draw_save(FRDraw* self);
void fr_draw_restore(FRDraw* self);


SYS_END_DECLS

#endif

