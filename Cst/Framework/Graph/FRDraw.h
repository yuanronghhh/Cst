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
  SysPointer v;

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
void fr_draw_get_size(FRDraw * self, SysInt * width, SysInt * height);
SYS_API FRContext* fr_draw_create_cr(FRDraw* self);
SYS_API FRSurface *fr_draw_get_surface(FRDraw *self);
FRContext* fr_draw_get_cr(FRDraw* self);
void fr_draw_stroke_mp(FRDraw* self, const FRRect *bound, FRSInt4* m4, FRSInt4* p4);
void fr_draw_fill_rectangle(FRDraw* self, const FRRect *bound);

SYS_END_DECLS

#endif

