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

  void (*construct) (FrDraw *o, FrIDevice *idevice);
};

struct _FrDraw {
  SysObject parent;

  /* <private> */
  FrContext *cr;
  FrSurface *idevice_surface;
  FrSurface *paint_surface;
  FrIDevice *idevice;
  SysBool is_painting;
};

SYS_API SysType fr_draw_get_type(void);
SYS_API FrDraw* fr_draw_new_I(FrIDevice *idevice);

SYS_API SysBool fr_draw_frame_need_draw(FrDraw *self);
SYS_API void fr_draw_frame_begin(FrDraw *self, FrRegion *region);
SYS_API void fr_draw_frame_end(FrDraw *self, FrRegion *region);

void fr_draw_setup(const SysChar *name, FrIDevice *idevice);
void fr_draw_teardown(void);
FrIDrawInterface* fr_draw_get_iface(void);

void fr_draw_set_cr(FrDraw *self, FrContext * cr);
FrContext * fr_draw_get_cr(FrDraw *self);

SYS_END_DECLS

#endif

